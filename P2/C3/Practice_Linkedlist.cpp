/*
 * Versión C++ de escritorio de la lista enlazada de eventos.
 * - Reemplaza tipos y llamadas Arduino por `std::string` y `std::cout`.
 * - Archivo permanece como `.cpp` y compila con g++/MSVC.
 */

#include <iostream>
#include <string>

// Definimos la estructura de un nodo para la lista enlazada
struct Nodo {
    std::string evento;   // Dato almacenado en el nodo (eventos en este caso)
    Nodo* siguiente;      // Puntero al siguiente nodo de la lista
};

// Puntero que apunta al primer nodo de la lista (inicialmente vacío)
Nodo* cabeza = nullptr;

// Función para agregar un nuevo evento a la lista enlazada
void agregarEvento(const std::string &nuevoEvento) {
    Nodo* nuevoNodo = new Nodo();
    nuevoNodo->evento = nuevoEvento;
    nuevoNodo->siguiente = cabeza;
    cabeza = nuevoNodo;
}

// Función para imprimir todos los eventos almacenados en la lista enlazada
void imprimirEventos() {
    Nodo* temp = cabeza;  // Puntero temporal para recorrer la lista
    std::cout << "Eventos guardados:\n";

    while (temp != nullptr) {
        std::cout << temp->evento << '\n';
        temp = temp->siguiente;
    }

    std::cout << "--------------------\n";
}

// Libera la memoria usada por la lista
void liberarEventos() {
    Nodo* temp = cabeza;
    while (temp != nullptr) {
        Nodo* siguiente = temp->siguiente;
        delete temp;
        temp = siguiente;
    }
    cabeza = nullptr;
}

// Ejecutable de demostración (reemplaza setup/loop de Arduino)
int main() {
    agregarEvento("Primer evento");
    agregarEvento("Segundo evento");

    imprimirEventos();

    liberarEventos();
    return 0;
}




