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

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(115200);
}

void moverAdelante() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, velocidadBase);
  analogWrite(ENB, velocidadBase);
}

void curvaIzquierda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, velocidadCurva);
  analogWrite(ENB, velocidadBase);
}

void curvaDerecha() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, velocidadBase);
  analogWrite(ENB, velocidadCurva);
}

void detener() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void loop() {
  // Invertimos lectura para que 1 = línea negra, 0 = fondo blanco
  int izq = digitalRead(sensorIzq);
  int der = digitalRead(sensorDer);

  Serial.print(izq);
  Serial.print(" ");
  Serial.println(der);

  // Ambos sensores sobre blanco (0) = avanzar
  if (izq == 0 && der == 0) {
    moverAdelante();
  }
  // Sensor izquierdo negro = gira izquierda
  else if (izq == 1 && der == 0) {
    curvaIzquierda();
  }
  // Sensor derecho negro = gira derecha
  else if (izq == 0 && der == 1) {
    curvaDerecha();
  }
  // Ambos sensores sobre negro = detener
  else {
    detener();
  }
}