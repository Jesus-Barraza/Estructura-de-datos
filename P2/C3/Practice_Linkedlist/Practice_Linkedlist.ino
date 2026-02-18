#include <Arduino.h>

// Inicializamos lo que se necesita para este proyecto
#define PIR_PIN 5
#define BTN_PIN 13

// Definimos la estructura de un nodo para la lista enlazada
struct Nodo {
    String evento;     // Dato almacenado en el nodo (eventos en este caso)
    Nodo* siguiente;   // Puntero al siguiente nodo de la lista
};

// Puntero que apunta al primer nodo de la lista (inicialmente vacío)
Nodo* cabeza = nullptr; 

// Función para agregar un nuevo evento a la lista enlazada
void agregarEvento(const String &nuevoEvento) {
    // Creación del nodo
    Nodo* nuevoNodo = new Nodo();
    nuevoNodo->evento = nuevoEvento;
    nuevoNodo->siguiente = cabeza;
    cabeza = nuevoNodo;
    
    // Comprobación de nodos
    Nodo* temp = cabeza;
    Nodo* ant = nullptr;
    int contador = 0;

    // Acción del contador
    while (temp != nullptr) {
        contador++;
        if (contador == 6) {
            ant->siguiente = nullptr;
            delete temp;
            break;
        }
        ant = temp;
        temp = temp->siguiente;
    }
}

// Función para imprimir todos los eventos almacenados en la lista enlazada
void imprimirEventos() {
    Nodo* temp = cabeza;
    Serial.println("Eventos guardados:");

    if (temp != nullptr) {
        while (temp != nullptr) {
            Serial.println(temp->evento);
            temp = temp->siguiente;
        }
    } else {
        Serial.println("No hay eventos guardados");
    }
    Serial.println("--------------------");
}

// Funciones del botón
void boton() {
    bool boton = digitalRead(BTN_PIN);
    static bool anterior = HIGH;


    if (boton == LOW && anterior == HIGH) {
        Serial.println("Lista de movimientos recientes (Los movimientos están separados por 300 microsegundos): ");
        imprimirEventos();
        delay(900);
    } else {
        deteccion();
    } 
    anterior = boton;
    delay(300);
}

//Funciones del detector de movimiento
void deteccion() {
    bool movimiento = digitalRead(PIR_PIN);
    
    if (movimiento) {
        agregarEvento("Se ha movido hace un momento");
    } else {
        agregarEvento("No se ha detectado movimiento");
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("");
    Serial.println("INICIO DEL PROGRAMA");
    // Se agregan eventos a la lista enlazada
    for (int i = 0; i<5; i++) {
        agregarEvento("...");
    }
    // Se imprimen los eventos almacenados
    imprimirEventos();

    // Se inicializan los componentes
    pinMode(PIR_PIN, INPUT);
    pinMode(BTN_PIN, INPUT_PULLUP);
}

void loop() {
    // El loop ya no está vacío xd
    boton();
}