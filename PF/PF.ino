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
int velocidadBase = 180;
int velocidadCurva = 5;

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
  digitalWrite(IN2, HIGH);  // Motor izq adelante

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  // Motor der adelante

  analogWrite(ENA, velocidadCurva);  // Izq LENTO
  analogWrite(ENB, velocidadBase);   // Der RÁPIDO → gira a la izquierda
}

void curvaDerecha() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  // Motor izq adelante

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  // Motor der adelante

  analogWrite(ENA, velocidadBase);   // Izq RÁPIDO → gira a la derecha
  analogWrite(ENB, velocidadCurva);  // Der LENTO
}

void detener() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void loop() {
  // Leer varias veces y promediar para estabilizar
  int izq = 0, der = 0, mid = 0;
  for(int i = 0; i < 5; i++) {
    izq += digitalRead(sensorIzq);
    der += digitalRead(sensorDer);
    mid += digitalRead(sensorEx);
    delay(2);
  }
  izq = izq / 5;
  der = der / 5;
  mid = mid / 5;

  Serial.print(izq); Serial.print(" ");
  Serial.print(mid); Serial.print(" ");
  Serial.println(der);

  if (mid == 1 && izq == 0 && der == 0) {
    moverAdelante();
  }
  else if (izq == 0 && der == 0) {
    moverAdelante();
  }
  else if (izq == 1 && der == 0) {
    curvaIzquierda();
  }
  else if (izq == 0 && der == 1) {
    curvaDerecha();
  }
  else if (izq == 1 && mid == 1 && der == 1) {
    detener();
  }
  else {
    detener();
  }
}