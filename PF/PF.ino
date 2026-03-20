// Pines sensores
const int sensorIzq = 32;
const int sensorDer = 35;
const int sensorEx = 34;

// Pines motores
const int IN1 = 27;
const int IN2 = 26;
const int IN3 = 25;
const int IN4 = 33;

const int ENA = 13;
const int ENB = 12;

// PWM Settings for older ESP32 Core
const int freq = 5000;
const int resolution = 8; 
const int canalA = 0; // PWM Channel 0
const int canalB = 1; // PWM Channel 1

// Velocidades
int velocidadBase = 200;
int velocidadCurva = 95;

void setup() {
  pinMode(sensorIzq, INPUT);
  pinMode(sensorDer, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Old Style PWM Setup
  ledcSetup(canalA, freq, resolution);
  ledcSetup(canalB, freq, resolution);
  
  ledcAttachPin(ENA, canalA);
  ledcAttachPin(ENB, canalB);

  Serial.begin(115200);
}

void moverAdelante() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  ledcWrite(canalA, velocidadBase); // Notice we use the CHANNEL, not the PIN
  ledcWrite(canalB, velocidadBase);
}

void curvaIzquierda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  ledcWrite(canalA, velocidadCurva);
  ledcWrite(canalB, velocidadBase);
}

void curvaDerecha() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  ledcWrite(canalA, velocidadBase);
  ledcWrite(canalB, velocidadCurva);
}

void detener() {
  ledcWrite(canalA, 0);
  ledcWrite(canalB, 0);
}

void loop() {
  int izq = digitalRead(sensorIzq);
  int der = digitalRead(sensorDer);

  Serial.print(izq);
  Serial.print(" ");
  Serial.println(der);

  if (izq == 0 && der == 0) {
    moverAdelante();
  }
  else if (izq == 1 && der == 0) {
    curvaIzquierda();
  }
  else if (izq == 0 && der == 1) {
    curvaDerecha();
  }
  else {
    detener();
  }
}