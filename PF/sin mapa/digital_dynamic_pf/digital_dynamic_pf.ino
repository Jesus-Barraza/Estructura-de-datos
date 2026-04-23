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

// Canales LEDC para PWM (ESP32)
const int canalA = 0;
const int canalB = 1;
const int pwmFreq = 1000;
const int pwmRes = 8;

// Constantes de control
int velocidadBase = 170;
bool stop = false;

enum Direccion { ADELANTE, CURVA_IZQ, CURVA_DER, FIN };

//constantes de la lógica
unsigned long inicioCorreccion = 0;
bool corrigiendo = false;
Direccion ultimaDir = ADELANTE;

struct Mapa {
  uint16_t numero;
  uint8_t registro_izq;
  uint8_t registro_cen;
  uint8_t registro_der;
  unsigned long timestamp;
  Direccion direccion;
  Mapa* siguiente;
};

int numero_registro = 0;
Direccion direccion_reg = ADELANTE;
Mapa* cabeza = nullptr;

void mapear(Mapa*& cabeza, int izq, int cen, int der, Direccion dir) {
  numero_registro++;
  unsigned long t = millis();
  Mapa* nodo = new (std::nothrow) Mapa{(uint16_t)numero_registro, (uint8_t)izq, (uint8_t)cen, (uint8_t)der, t, dir, cabeza};
  cabeza = nodo;
}

void desMapear(Mapa*& cabeza) {
  while (cabeza) {
    Mapa* tmp = cabeza;
    cabeza = cabeza->siguiente;
    delete tmp;
  }
  numero_registro = 0;
}

int last_izq = -1, last_mid = -1, last_der = -1;
unsigned long last_map_time = 0;
const unsigned long MAP_INTERVAL_MS = 200;

void cambio(Mapa*& cabeza, int izq, int mid, int der, Direccion status) {
  unsigned long now = millis();
  if (izq == last_izq && mid == last_mid && der == last_der) {
    if (now - last_map_time < MAP_INTERVAL_MS) return;
  }
  last_izq = izq; last_mid = mid; last_der = der;
  last_map_time = now;
  mapear(cabeza, izq, mid, der, status);
}

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
  desMapear(cabeza);
}

void moverAdelante() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  analogWrite(ENA, constrain(velocidadBase, 0, 255));
  analogWrite(ENB, constrain(velocidadBase, 0, 255));
}

void curvaIzquierda() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, constrain(velocidadBase - 54, 0, 255));
  analogWrite(ENB, constrain(velocidadBase + 20, 0, 255));
}

void curvaDerecha() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  analogWrite(ENA, constrain(velocidadBase + 54, 0, 255));
  analogWrite(ENB, constrain(velocidadBase - 40, 0, 255));
}

void detener() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void loop() {
  if (stop) return;

  int izq = digitalRead(sensorIzq);
  int der = digitalRead(sensorDer);
  int mid = digitalRead(sensorEx);

  Serial.print("IZQ: "); Serial.print(izq);
  Serial.print(" | MID: "); Serial.print(mid);
  Serial.print(" | DER: "); Serial.println(der);

  bool izqDetecta = (izq == HIGH);
  bool derDetecta = (der == HIGH);
  bool midDetecta = (mid == HIGH);

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
      stop = true;
      corrigiendo = false;
    }
    return;
  }

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
  } else {
    static unsigned long espera = millis();
    while ((millis() - espera < 1000) && (izqDetecta || midDetecta || derDetecta)) {
      if ((millis() - espera < 1000) && (izqDetecta || midDetecta || derDetecta)) {
        stop = false;
      }
    }
  }
}