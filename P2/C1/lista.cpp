#include <iostream>
#include <string>
using namespace std;

// Clase Nodo
class Nodo {
    public:
        string dato;      // Texto dinámico (como char*)
        Nodo* siguiente;

    Nodo(string texto) {
        dato = texto;
        siguiente = nullptr;
    }
};

// Clase ListaEnlazada
class ListaEnlazada {
    private:
        Nodo* cabeza;

    public:
        ListaEnlazada() {
            cabeza = nullptr;
        }

    // Agregar al final
    void agregar(string texto) {
        Nodo* nuevo_nodo = new Nodo(texto);

        if (cabeza == nullptr) {
            cabeza = nuevo_nodo;
            return;
        }

        Nodo* actual = cabeza;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }

        actual->siguiente = nuevo_nodo;
    }

    // Mostrar lista
    void mostrar() {
        Nodo* actual = cabeza;

        while (actual != nullptr) {
            cout << actual->dato << endl;
            actual = actual->siguiente;
        }
    }

    // Destructor (libera memoria automáticamente)
    ~ListaEnlazada() {
        Nodo* actual = cabeza;
        Nodo* temp;

        while (actual != nullptr) {
            temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
};

int main() {
    ListaEnlazada tareas;

    tareas.agregar("Comprar pan");
    tareas.agregar("Enviar correo");
    tareas.agregar("Estudiar listas enlazadas en C++");

    cout << "Lista de tareas:" << endl;
    tareas.mostrar();

    return 0;
}
