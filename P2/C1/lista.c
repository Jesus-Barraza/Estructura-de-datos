#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definición del nodo
typedef struct Nodo {
    char* dato;              // Cadena dinámica
    struct Nodo* siguiente;
} Nodo;

// Función para agregar al final
void agregar(Nodo** cabeza, const char* texto) {
    // Crear nuevo nodo
    Nodo* nuevo_nodo = (Nodo*) malloc(sizeof(Nodo));
    if (nuevo_nodo == NULL) {
        printf("Error al asignar memoria para el nodo.\n");
        return;
    }

    // Reservar memoria exacta para la cadena
    nuevo_nodo->dato = (char*) malloc(strlen(texto) + 1);
    if (nuevo_nodo->dato == NULL) {
        printf("Error al asignar memoria para la cadena.\n");
        free(nuevo_nodo);
        return;
    }

    strcpy(nuevo_nodo->dato, texto);
    nuevo_nodo->siguiente = NULL;

    // Si la lista está vacía
    if (*cabeza == NULL) {
        *cabeza = nuevo_nodo;
        return;
    }

    // Recorrer hasta el final
    Nodo* actual = *cabeza;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }

    actual->siguiente = nuevo_nodo;
}

// Mostrar lista
void mostrar(Nodo* cabeza) {
    Nodo* actual = cabeza;

    while (actual != NULL) {
        printf("%s\n", actual->dato);
        actual = actual->siguiente;
    }
}

// Liberar lista completamente
void liberarLista(Nodo* cabeza) {
    Nodo* temp;

    while (cabeza != NULL) {
        temp = cabeza;
        cabeza = cabeza->siguiente;

        free(temp->dato);   // Liberar la cadena primero
        free(temp);         // Luego el nodo
    }
}

int main() {
    Nodo* cabeza = NULL;

    agregar(&cabeza, "Comprar pan");
    agregar(&cabeza, "Enviar correo");
    agregar(&cabeza, "Estudiar listas enlazadas en C");

    printf("Lista de tareas:\n");
    mostrar(cabeza);

    liberarLista(cabeza);

    return 0;
}
