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

// Constantes de control
int velocidadBase = 200;
int trimIzq = 0;   // ajustar si se va a la derecha
int trimDer = 10;  // ajustar si se va a la izquierda
int threshold = 900; // ajustar según lecturas del serial
bool stop = false; // verifica que el carrito se detuvo y deja de registrar

//Enumeración de los textos (por ende la dirección lol)
enum Direccion { ADELANTE, CURVA_IZQ, CURVA_DER, FIN };

//Estructura de mapeado (ya veremos cómo guardar estos) + número de registro
struct Mapa {
  uint16_t numero;
  uint16_t registro_izq;
  uint16_t registro_cen;
  uint16_t registro_der;
  unsigned long timestamp;
  Direccion direccion;
  Mapa* siguiente;
};

int numero_registro = 0;
Direccion direccion_reg;
Mapa* cabeza = nullptr;

//Funciones de mapa
void mapear(Mapa*& cabeza, int izq, int cen, int der, Direccion dir) {
  numero_registro++;
  unsigned long t = millis();
  Mapa* nodo = new Mapa{(uint16_t)numero_registro, (uint16_t)izq, (uint16_t)cen, (uint16_t)der, t, dir, cabeza};
  if (!nodo) return;
  cabeza = nodo;
}

void desMapear(Mapa*& cabeza) {
  while (cabeza) {
    Mapa* tmp = cabeza;
    cabeza = cabeza->siguiente;
    delete tmp;
  }
  numero_registro = 0;
};

//Controles de guardado
int last_izq = -1, last_mid = -1, last_der = -1;
unsigned long last_map_time = 0;
const unsigned long MAP_INTERVAL_MS = 200;

//Función de guardado
void cambio(Mapa*& cabeza, int izq, int mid, int der, Direccion status) {
  unsigned long now = millis();
  if (izq == last_izq && mid == last_mid && der == last_der) {
    if (now - last_map_time < MAP_INTERVAL_MS) return;
  }
  last_izq = izq; last_mid = mid; last_der = der;
  last_map_time = now;
  mapear(cabeza, izq, mid, der, status);
}


//Funciones del carrito
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
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);

  analogWrite(ENA, constrain(velocidadBase, 0, 255));
  analogWrite(ENB, constrain(velocidadBase, 0, 255));
}

void curvaIzquierda(int izqVal, int derVal) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);

  int correccion = map(izqVal, 0, threshold, 60, 0);
  analogWrite(ENA, constrain(velocidadBase - correccion, 0, 255));
  analogWrite(ENB, constrain(velocidadBase + correccion / 2, 0, 255));
}

void curvaDerecha(int derVal, int izqVal) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);

  int correccion = map(derVal, 0, threshold, 60, 0);
  analogWrite(ENA, constrain(velocidadBase + correccion / 2, 0, 255));
  analogWrite(ENB, constrain(velocidadBase - correccion, 0, 255));
}

void detener() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void loop() {
  int izq = analogRead(sensorIzq);
  int der = analogRead(sensorDer);
  int mid = analogRead(sensorEx);

  Serial.print("IZQ: "); Serial.print(izq);
  Serial.print(" | MID: "); Serial.print(mid);
  Serial.print(" | DER: "); Serial.println(der);

  bool izqDetecta = (izq > threshold);
  bool derDetecta = (der > threshold);
  bool midDetecta = (mid > threshold);

  if (!izqDetecta && !midDetecta && !derDetecta) {
    detener();
    direccion_reg = FIN;
    cambio(cabeza, izq, mid, der, direccion_reg);
    //stop = true;
  }
  else if (izqDetecta && !derDetecta) {
    curvaIzquierda(izq, der);
    direccion_reg = CURVA_IZQ;
  }
  else if (!izqDetecta && derDetecta) {
    curvaDerecha(der, izq);
    direccion_reg = CURVA_DER;
  }
  else {
    moverAdelante();
    direccion_reg = ADELANTE;
  }

  delay(100);

  if (!stop && (izqDetecta || midDetecta || derDetecta)) {
    cambio(cabeza, izq, mid, der, direccion_reg);
  }
}