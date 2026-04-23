// Pines sensores
const int sensorIzq = 32;
const int sensorDer = 35;
const int sensorEx  = 34;

// Pines motores
const int IN1 = 27;
const int IN2 = 14;
const int IN3 = 26;
const int IN4 = 25;

const int ENA = 13;
const int ENB = 12;

// Parámetros PWM "virtual"
const int pwmFreq = 1000;   // Hz
const int pwmRes  = 8;      // resolución 8 bits -> 0..255

// Constantes de control
int velocidadBase = 170;
bool stop = false;

enum Direccion { ADELANTE, CURVA_IZQ, CURVA_DER, GIRO_IZQ, GIRO_DER, FIN };

// --- Estructura Mapa ---
struct Mapa {
  uint16_t numero;
  uint8_t registro_izq;
  uint8_t registro_cen;
  uint8_t registro_der;
  unsigned long timestamp;
  Direccion direccion;
  Mapa* siguiente;
};

// --- Buffer circular ---
const int MAP_SIZE = 256;
static Mapa mapaBuffer[MAP_SIZE];
static int map_head = 0;
static int map_count = 0;
Mapa* cabeza = nullptr;
int numero_registro = 0;

// Variables de corrección
unsigned long inicioCorreccion = 0;
bool corrigiendo = false;
Direccion ultimaDir = ADELANTE;

// Variables de control del mapeado
int last_izq = -1, last_mid = -1, last_der = -1;
unsigned long last_map_time = 0;
const unsigned long MAP_INTERVAL_MS = 200;
unsigned long ultimaComprobacionStop = 0;
Direccion direccion_reg = ADELANTE;

// --- Soft PWM ---
struct SoftPWM {
  int pin;
  uint8_t value;
};

SoftPWM pwmA = { ENA, 0 };
SoftPWM pwmB = { ENB, 0 };

const unsigned long pwmPeriodUs = 1000000UL / pwmFreq;

void updateSoftPwm() {
  unsigned long t = micros();
  unsigned long phase = t % pwmPeriodUs;

  // canal A
  if (pwmA.value == 0) digitalWrite(pwmA.pin, LOW);
  else if (pwmA.value >= 255) digitalWrite(pwmA.pin, HIGH);
  else {
    unsigned long onTimeA = (unsigned long)pwmA.value * pwmPeriodUs / 255UL;
    digitalWrite(pwmA.pin, (phase < onTimeA) ? HIGH : LOW);
  }

  // canal B
  if (pwmB.value == 0) digitalWrite(pwmB.pin, LOW);
  else if (pwmB.value >= 255) digitalWrite(pwmB.pin, HIGH);
  else {
    unsigned long onTimeB = (unsigned long)pwmB.value * pwmPeriodUs / 255UL;
    digitalWrite(pwmB.pin, (phase < onTimeB) ? HIGH : LOW);
  }
}

void analogWrite(int pin, int value) {
  value = constrain(value, 0, 255);
  if (pin == pwmA.pin) pwmA.value = (uint8_t)value;
  else if (pin == pwmB.pin) pwmB.value = (uint8_t)value;
  else digitalWrite(pin, value ? HIGH : LOW);
}

// --- Funciones de movimiento ---
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

// --- Funciones de giro completo ---
void girarIzquierda() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);   // motor A atrás
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);  // motor B adelante
  analogWrite(ENA, velocidadBase);
  analogWrite(ENB, velocidadBase);
}

void girarDerecha() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);  // motor A adelante
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);   // motor B atrás
  analogWrite(ENA, velocidadBase);
  analogWrite(ENB, velocidadBase);
}

// --- Mapeo ---
void mapear(Mapa*& cabeza, int izq, int cen, int der, Direccion dir) {
  numero_registro++;
  unsigned long t = millis();
  Mapa* nodo = &mapaBuffer[map_head];
  nodo->numero = numero_registro;
  nodo->registro_izq = izq ? 1 : 0;
  nodo->registro_cen = cen ? 1 : 0;
  nodo->registro_der = der ? 1 : 0;
  nodo->timestamp = t;
  nodo->direccion = dir;
  nodo->siguiente = cabeza;
  cabeza = nodo;
  map_head = (map_head + 1) % MAP_SIZE;
  if (map_count < MAP_SIZE) map_count++;
}

void cambio(Mapa*& cabeza, int izq, int mid, int der, Direccion status) {
  unsigned long now = millis();
  if (izq == last_izq && mid == last_mid && der == last_der) {
    if (now - last_map_time < MAP_INTERVAL_MS) return;
  }
  last_izq = izq; last_mid = mid; last_der = der;
  last_map_time = now;
  mapear(cabeza, izq, mid, der, status);
}

const char* direccionToString(Direccion d) {
  switch (d) {
    case ADELANTE:   return "ADELANTE";
    case CURVA_IZQ:  return "CURVA_IZQ";
    case CURVA_DER:  return "CURVA_DER";
    case GIRO_IZQ:   return "GIRO_IZQ";
    case GIRO_DER:   return "GIRO_DER";
    case FIN:        return "FIN";
    default:         return "DESCONOCIDA";
  }
}

// --- Setup ---
void setup() {
  pinMode(sensorIzq, INPUT);
  pinMode(sensorDer, INPUT);
  pinMode(sensorEx, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(115200);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// --- Loop principal ---
void loop() {
  updateSoftPwm();

  if (stop) return;

  int izq = digitalRead(sensorIzq);
  int der = digitalRead(sensorDer);
  int mid = digitalRead(sensorEx);

  bool izqDetecta = (izq == HIGH);
  bool derDetecta = (der == HIGH);
  bool midDetecta = (mid == HIGH);

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

  if (!stop && (izqDetecta || midDetecta || derDetecta)) {
    cambio(cabeza, izq, mid, der, direccion_reg);
  }
}
