//Estructura
struct Orden {
  int numOrden;
  String nombre;
  Orden* siguiente;  
};

//Valores universales y de componentes
int LEDs[] = {2, 5};
int numLeds = 2;
int BTNs[] = {18, 19};
int numBtns = 2;

int limitTimer = 1000;

int contador = 0;
Orden* cabeza = nullptr;

String nombres[3] = {"Carlos", "Laura", "Joaquín"};
int contNombres = 3;

int postBtns[] = {HIGH, HIGH};
int preBtns[] = {HIGH, HIGH};

//Orden de Queue
void orden() {
  contador++;
  Orden* lista = new Orden;
  lista->numOrden = contador;
  lista->nombre = nombres[contador%contNombres];
  lista->siguiente = nullptr;

  if (cabeza == nullptr) {
    cabeza = lista;
  } else {
    Orden* temp = cabeza;
    while (temp->siguiente != nullptr) {
      temp = temp->siguiente;
    }
    temp->siguiente = lista;
  }
};

void recogido() {
  if (cabeza != nullptr) {
    Orden* temp = cabeza;
    cabeza = cabeza->siguiente;
    delete temp;
    contador--;

    Orden* temp2 = cabeza;
    while (temp2 != nullptr) {
      temp2->numOrden--;
      temp2 = temp2->siguiente;
    }
  }
}

//Impresión
void imprimir() {
  Orden* temp = cabeza;
  Serial.println("--------------");
  while (temp != nullptr) {
    Serial.print("Orden n° ");
    Serial.print(temp->numOrden);
    Serial.print(": ");
    Serial.println(temp->nombre);
    temp = temp->siguiente;
  }
  Serial.println("--------------");
}

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < numLeds; i++) {
    pinMode(LEDs[i], OUTPUT);
  }

  for (int i = 0; i < numBtns; i++) {
    pinMode(BTNs[i], INPUT_PULLUP);
  }

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < numBtns; i++) {
    postBtns[i] = digitalRead(BTNs[i]);
    if (postBtns[i] == LOW && preBtns[i] == HIGH) {
      if (i == 0) {
        orden();
      } else {
        recogido();
      }
    }
    preBtns[i] = postBtns[i];
  }

  if (cabeza != nullptr) {
    digitalWrite(LEDs[0], HIGH);   
  } else {
    digitalWrite(LEDs[0], LOW);    
  }

  if (contador > 3) {
    digitalWrite(LEDs[1], HIGH);
  } else {
    digitalWrite(LEDs[1], LOW);
  }

  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= limitTimer) {
    imprimir();
    lastPrint = millis();
  }
}
