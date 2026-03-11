#include <TimeLib.h>

//estructura de nodo otra vez por la pu-
struct Emergencia {
  String instruction;
  time_t tiempo;
  Emergencia* siguiente;
};

//Definiciones
int boton[] = {15, 18, 19};
int numBtn = 3;
int led = 4;

int antBtn[] = {HIGH, HIGH, HIGH};
int postBtn[] = {HIGH, HIGH, HIGH};
bool var = false;

Emergencia* cabeza = nullptr; 

//Función para insertar los datos
bool enqueue(int btn) {
  String msg;
  if (btn == 0) {
    msg = "Fire"; 
  };
  if (btn == 1) {
    msg = "Intrusion";
  } else {
    msg = "Electrical failure";
  };

  Emergencia* nuevo = new Emergencia;
  nuevo->instruction = msg;
  nuevo->tiempo = now();
  nuevo->siguiente = nullptr;

  if (cabeza == nullptr) {
    cabeza=nuevo;
  } else {
    Emergencia* temp = cabeza;
    while (temp->siguiente != nullptr) {
      temp = temp->siguiente;
    }
    temp->siguiente = nuevo;
  }

  return true;
};

//Función para eliminar los datos
void dequeue() {
  if (cabeza != nullptr) {
    digitalWrite(led, HIGH);
    while (cabeza != nullptr) {
      Serial.println("------------------");
      Serial.println("Se ha detectado una alarma \n");
      Serial.print("Tipo: ");
      Serial.println(cabeza->instruction);
      Serial.print("Fecha: ");
      Serial.print(day(cabeza->tiempo));
      Serial.print("/");
      Serial.print(month(cabeza->tiempo));
      Serial.print("/");
      Serial.print(year(cabeza->tiempo));
      Serial.print(" ");
      Serial.print(hour(cabeza->tiempo));
      Serial.print(":");
      Serial.print(minute(cabeza->tiempo));
      Serial.print(":");
      Serial.println(second(cabeza->tiempo));
      Serial.println("------------------");

      int contador = 0;
      Emergencia* temp2 = cabeza -> siguiente;
      while (temp2 != nullptr) {
        temp2 = temp2->siguiente;
        contador++;
      }
      Serial.print("Todavía quedan ");
      Serial.print(contador);
      Serial.println(" alertas");
      Serial.println("------------------");

      delay(3000);

      Emergencia* temp = cabeza;
      cabeza = cabeza->siguiente;
      delete temp;
    }
    digitalWrite(led, LOW);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setTime(19,22,0,10,3,2026);
  for (int i = 0; i < numBtn; i++) {
    pinMode(boton[i], INPUT_PULLUP);
  };
  pinMode(led, OUTPUT);
}

//Timers
unsigned long tiempoInicio = 0;
unsigned long intervalo = 10000;

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < numBtn; i++) {
    postBtn[i] = digitalRead(boton[i]);
    if (antBtn[i] == HIGH && postBtn[i] == LOW) {
      var = enqueue(i);
      if (tiempoInicio == 0) {
        tiempoInicio = millis();
      }
    }
    antBtn[i] = postBtn[i];
  };

  if (var && cabeza != nullptr && (millis() - tiempoInicio >= intervalo)) {
    dequeue();
    var = false;
    tiempoInicio = 0;
  }
}
