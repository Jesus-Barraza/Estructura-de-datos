// Pines sensores
const int sensorIzq = 32;
const int sensorDer = 35;
const int sensorEx = 34;

// Pines motores
const int IN1 = 27;
const int IN2 = 14;
const int IN3 = 26;
const int IN4 = 25;

const int ENA = 13;
const int ENB = 12;

// Parámetros PWM "virtual"
const int pwmFreq = 1000;   // Hz (periodo de 1 ms)
const int pwmRes = 8;       // resolución 8 bits -> 0..255

// Constantes de control
int velocidadBase = 170;
bool stop = false;

enum Direccion { ADELANTE, CURVA_IZQ, CURVA_DER, FIN };

// constantes de la lógica
unsigned long inicioCorreccion = 0;
bool corrigiendo = false;
Direccion ultimaDir = ADELANTE;

// --- Estructura Mapa (se mantiene igual) ---
struct Mapa {
  uint16_t numero;
  uint8_t registro_izq;
  uint8_t registro_cen;
  uint8_t registro_der;
  unsigned long timestamp;
  Direccion direccion;
  Mapa* siguiente;
};

// --- Buffer circular para mapeado ---
const int MAP_SIZE = 256;               // ajustar según memoria disponible
static Mapa mapaBuffer[MAP_SIZE];
static int map_head = 0;                // índice donde se escribirá el siguiente registro
static int map_count = 0;               // cantidad de registros actualmente almacenados

// temporizador para reactivar stop (no bloqueante)
unsigned long ultimaComprobacionStop = 0;

// variables de control del mapeado
int numero_registro = 0;
Direccion direccion_reg = ADELANTE;
Mapa* cabeza = nullptr;

int last_izq = -1, last_mid = -1, last_der = -1;
unsigned long last_map_time = 0;
const unsigned long MAP_INTERVAL_MS = 200;

// --- mapear: escribe en el buffer circular (sin new/delete) ---
void mapear(Mapa*& cabeza, int izq, int cen, int der, Direccion dir) {
  numero_registro++;
  unsigned long t = millis();

  // obtener nodo destino en el buffer
  Mapa* nodo = &mapaBuffer[map_head];

  // rellenar campos (manteniendo tipos del struct)
  nodo->numero = (uint16_t)numero_registro;
  nodo->registro_izq = (uint8_t)(izq ? 1 : 0);
  nodo->registro_cen = (uint8_t)(cen ? 1 : 0);
  nodo->registro_der = (uint8_t)(der ? 1 : 0);
  nodo->timestamp = t;
  nodo->direccion = dir;

  // mantener compatibilidad: apuntar 'siguiente' a la cabeza previa
  nodo->siguiente = cabeza;
  cabeza = nodo;

  // avanzar cabeza del buffer circular
  map_head = (map_head + 1) % MAP_SIZE;

  // actualizar contador (si está lleno, sobrescribimos el más antiguo)
  if (map_count < MAP_SIZE) {
    map_count++;
  } else {
    // sobrescritura: nada más que hacer en esta implementación
  }
}

// --- desMapear: reinicia el buffer lógico ---
void desMapear(Mapa*& cabeza) {
  cabeza = nullptr;
  map_head = 0;
  map_count = 0;
  numero_registro = 0;

  last_izq = last_mid = last_der = -1;
  last_map_time = 0;
  ultimaComprobacionStop = 0;
}

// --- cambio: controla la frecuencia de registro y llama a mapear ---
void cambio(Mapa*& cabeza, int izq, int mid, int der, Direccion status) {
  unsigned long now = millis();

  if (izq == last_izq && mid == last_mid && der == last_der) {
    if (now - last_map_time < MAP_INTERVAL_MS) return;
  }
  last_izq = izq; last_mid = mid; last_der = der;
  last_map_time = now;
  mapear(cabeza, izq, mid, der, status);
}

// ------------------ Implementación simple de analogWrite (software PWM) ------------------
// Esta implementación evita el uso de ledc y proporciona analogWrite(pin, value)
// con resolución 8 bits (0..255). Está pensada para controlar un número pequeño
// de pines PWM (aquí usamos ENA y ENB). El PWM se actualiza en loop() mediante micros().

struct SoftPWM {
  int pin;
  uint8_t value;        // 0..255
};

SoftPWM pwmA = { ENA, 0 };
SoftPWM pwmB = { ENB, 0 };

const unsigned long pwmPeriodUs = 1000000UL / pwmFreq; // periodo en microsegundos

// Llamar frecuentemente desde loop para aplicar el PWM
void updateSoftPwm() {
  unsigned long t = micros();
  unsigned long phase = t % pwmPeriodUs;

  // canal A
  if (pwmA.value == 0) {
    digitalWrite(pwmA.pin, LOW);
  } else if (pwmA.value >= 255) {
    digitalWrite(pwmA.pin, HIGH);
  } else {
    unsigned long onTimeA = (unsigned long)pwmA.value * pwmPeriodUs / 255UL;
    digitalWrite(pwmA.pin, (phase < onTimeA) ? HIGH : LOW);
  }

  // canal B
  if (pwmB.value == 0) {
    digitalWrite(pwmB.pin, LOW);
  } else if (pwmB.value >= 255) {
    digitalWrite(pwmB.pin, HIGH);
  } else {
    unsigned long onTimeB = (unsigned long)pwmB.value * pwmPeriodUs / 255UL;
    digitalWrite(pwmB.pin, (phase < onTimeB) ? HIGH : LOW);
  }
}

// analogWrite pública (0..255)
void analogWrite(int pin, int value) {
  value = constrain(value, 0, 255);
  if (pin == pwmA.pin) {
    pwmA.value = (uint8_t)value;
  } else if (pin == pwmB.pin) {
    pwmB.value = (uint8_t)value;
  } else {
    // Si se llama a analogWrite para otro pin, intentar usar digitalWrite como fallback
    // (no genera PWM)
    digitalWrite(pin, value ? HIGH : LOW);
  }
}
// --------------------------------------------------------------------

// --- Setup y control de motores ---
void setup() {
  pinMode(sensorIzq, INPUT);
  pinMode(sensorDer, INPUT);
  pinMode(sensorEx, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // ENA y ENB se usan con software PWM: configurar como OUTPUT
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(115200);
  desMapear(cabeza);

  // Inicializar salidas PWM a 0 usando analogWrite
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void moverAdelante() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  analogWrite(ENA, constrain(velocidadBase + 40, 0, 255));
  analogWrite(ENB, constrain(velocidadBase + 40, 0, 255));
}

void curvaIzquierda() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, constrain(velocidadBase - 50, 0, 255));
  analogWrite(ENB, constrain(velocidadBase + 40, 0, 255));
}

void curvaDerecha() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  analogWrite(ENA, constrain(velocidadBase + 40, 0, 255));
  analogWrite(ENB, constrain(velocidadBase - 50, 0, 255));
}

void detener() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// --- Loop principal (no bloqueante) ---
void loop() {
  // Actualizar PWM software al inicio de cada ciclo
  updateSoftPwm();

  // Si stop está activo, no ejecutamos la lógica principal
  if (stop) return;

  // Leer sensores (guardar millis localmente si se usa varias veces)
  int izq = digitalRead(sensorIzq);
  int der = digitalRead(sensorDer);
  int mid = digitalRead(sensorEx);

  Serial.print("IZQ: "); Serial.print(izq);
  Serial.print(" | MID: "); Serial.print(mid);
  Serial.print(" | DER: "); Serial.println(der);

  bool izqDetecta = (izq == HIGH);
  bool derDetecta = (der == HIGH);
  bool midDetecta = (mid == HIGH);

  // --- Manejo de corrección no bloqueante ---
  if (corrigiendo) {
    if (millis() - inicioCorreccion < 800) {
      if (ultimaDir == CURVA_DER) {
        curvaIzquierda();
        direccion_reg = CURVA_IZQ;
      } else {
        curvaDerecha();
        direccion_reg = CURVA_DER;
      }

      int ri = digitalRead(sensorIzq);
      int rm = digitalRead(sensorEx);
      int rd = digitalRead(sensorDer);

      cambio(cabeza, ri, rm, rd, direccion_reg);

      if (ri == HIGH || rm == HIGH || rd == HIGH) {
        detener();
        direccion_reg = FIN;
        corrigiendo = false;
      }
    } else {
      // no encontró la línea en el tiempo de corrección: activar stop y empezar temporizador
      stop = true;
      corrigiendo = false;
      ultimaComprobacionStop = millis();
    }
    return; // salir para no ejecutar la lógica normal en este ciclo
  }

  // --- Lógica normal de seguimiento de línea ---
  if (!izqDetecta && !midDetecta && !derDetecta) {
    detener();
    ultimaDir = direccion_reg;
    inicioCorreccion = millis();
    corrigiendo = true;
  } else if (izqDetecta && !derDetecta) {
    curvaIzquierda();
    direccion_reg = CURVA_IZQ;
  } else if (!izqDetecta && derDetecta) {
    curvaDerecha();
    direccion_reg = CURVA_DER;
  } else if (izqDetecta && midDetecta && derDetecta) {
    moverAdelante();
    direccion_reg = ADELANTE;
  } else {
    moverAdelante();
    direccion_reg = ADELANTE;
  }

  // --- Registro de lecturas (mapa) ---
  if (!stop && (izqDetecta || midDetecta || derDetecta)) {
    cambio(cabeza, izq, mid, der, direccion_reg);
  }

  // --- Lógica no bloqueante para reactivar stop ---
  if (ultimaComprobacionStop != 0) {
    // si han pasado al menos 1000 ms desde que se activó stop y ahora hay detección, reactivar
    if ((millis() - ultimaComprobacionStop >= 1000) && (izqDetecta || midDetecta || derDetecta)) {
      stop = false;
      ultimaComprobacionStop = 0;
    }
    // si stop fue desactivado por otra razón, reiniciar temporizador
    if (!stop) {
      ultimaComprobacionStop = 0;
    }
  }
}
