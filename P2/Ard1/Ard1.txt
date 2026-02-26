//Características de componentes
int LEDs[] = {4, 5, 15, 18, 19};
int numLED = sizeof(LEDs) / sizeof(LEDs[0]);
int BTNs[] = {22, 23};
int numBTN = sizeof(BTNs) / sizeof(BTNs[0]);

//Estructura de nodos
struct Nodo {
  int id;
  Nodo* cadena;
};

//Otros valores globales
Nodo* cabeza = nullptr;
static int contadorvite = 0;

//Función de la lista
void anadirLuces(const int &ide) {
  //ver nodo
  Nodo* luz = new Nodo();
  luz->id = ide;
  luz->cadena = cabeza;
  cabeza = luz;

  //comprobar cadena
  Nodo* temp = cabeza;
  Nodo* ant = nullptr;
  int contador = 0;

  //accion
  while (temp != nullptr) {
    contador++;
    if (contador >= numLED+1) {
      ant->cadena = nullptr;
      delete temp;
      break;
    }
    ant = temp;
    temp = temp->cadena;
  };
};

void borrarLuces() {
  if (cabeza != nullptr) {
    Nodo* temp = cabeza;
    cabeza = cabeza->cadena;
    delete temp;
  };
};

//Función de las luces
void luces() {
  for (int i = 0; i < numLED; i++) {
    digitalWrite(LEDs[i], LOW);
  }
  Nodo* temp = cabeza;
  while (temp != nullptr) {
    digitalWrite(temp->id, HIGH);
    temp = temp->cadena;
  }
};

//Función del botón
void boton() {
  bool btn1 = digitalRead(BTNs[0]);
  static bool antbtn1 = HIGH;
  bool btn2 = digitalRead(BTNs[1]);
  static bool antbtn2 = HIGH;

  if (btn1 == LOW && antbtn1 == HIGH && contadorvite < numLED) {
    anadirLuces(LEDs[contadorvite]);
    contadorvite++;
    luces();
    delay(300);
  }
  if (btn2 == LOW && antbtn2 == HIGH && contadorvite > 0) {
    borrarLuces();
    contadorvite--;
    luces();
    delay(300);
  };

  antbtn1 = btn1;
  antbtn2 = btn2;
};

//setup
void setup() {
  for (int i = 0; i < numLED; i++) {
    pinMode(LEDs[i], OUTPUT);
  }
  for (int i = 0; i < numBTN; i++) {
    pinMode(BTNs[i], INPUT_PULLUP);
  }
}

void loop() {
  //Borré esto sin querer, ups
  boton();
}
