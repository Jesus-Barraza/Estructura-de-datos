/*
 * Implementación en C de una lista enlazada simple que almacena eventos (cadenas).
 * - Usa `malloc`/`free` y `char*` para ser compatible con compiladores C.
 * - Contiene funciones: agregarEvento, imprimirEventos y liberarEventos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nodo {
    char *evento;            /* cadena dinámica que guarda el evento */
    struct Nodo *siguiente;  /* puntero al siguiente nodo */
} Nodo;

/* Puntero a la cabeza de la lista (inicialmente vacío) */
Nodo *cabeza = NULL;

/* Añade un nuevo evento al principio de la lista */
void agregarEvento(const char *nuevoEvento) {
    Nodo *nuevoNodo = (Nodo *)malloc(sizeof(Nodo));
    if (!nuevoNodo) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    size_t len = strlen(nuevoEvento) + 1;
    nuevoNodo->evento = (char *)malloc(len);
    if (!nuevoNodo->evento) {
        free(nuevoNodo);
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    memcpy(nuevoNodo->evento, nuevoEvento, len);

    nuevoNodo->siguiente = cabeza;
    cabeza = nuevoNodo;
}

/* Imprime todos los eventos almacenados en la lista */
void imprimirEventos(void) {
    Nodo *temp = cabeza;
    puts("Eventos guardados:");
    while (temp != NULL) {
        puts(temp->evento);
        temp = temp->siguiente;
    }
    puts("--------------------");
}

/* Libera toda la memoria usada por la lista */
void liberarEventos(void) {
    Nodo *temp = cabeza;
    while (temp != NULL) {
        Nodo *siguiente = temp->siguiente;
        free(temp->evento);
        free(temp);
        temp = siguiente;
    }
    cabeza = NULL;
}

/* Demostración simple usando main() */
int main(void) {
    agregarEvento("Primer evento");
    agregarEvento("Segundo evento");

    imprimirEventos();

    liberarEventos();
    return 0;
}




