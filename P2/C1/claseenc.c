#include <stdio.h>
#include <stdlib.h>

// Definición de la "clase" Rectangulo
typedef struct Rectangulo {
    double ancho;
    double alto;

    // Métodos como punteros a funciones
    double (*area)(struct Rectangulo *self);
    double (*perimetro)(struct Rectangulo *self);
} Rectangulo;

// Implementación de métodos
double rectangulo_area(Rectangulo *self) {
    return self->ancho * self->alto;
}

double rectangulo_perimetro(Rectangulo *self) {
    return 2 * (self->ancho + self->alto);
}

// "Constructor"
Rectangulo *Rectangulo_nuevo(double ancho, double alto) {
    Rectangulo *r = (Rectangulo *)malloc(sizeof(Rectangulo));
    if (!r) {
        fprintf(stderr, "Error: no se pudo asignar memoria.\n");
        exit(EXIT_FAILURE);
    }
    r->ancho = ancho;
    r->alto = alto;
    r->area = rectangulo_area;
    r->perimetro = rectangulo_perimetro;
    return r;
}

// "Destructor"
void Rectangulo_destruir(Rectangulo *r) {
    free(r);
}

int main() {
    // Crear un objeto Rectangulo
    Rectangulo *miRect = Rectangulo_nuevo(5.0, 3.0);

    // Usar métodos
    printf("Ancho: %.2f\n", miRect->ancho);
    printf("Alto: %.2f\n", miRect->alto);
    printf("Área: %.2f\n", miRect->area(miRect));
    printf("Perímetro: %.2f\n", miRect->perimetro(miRect));

    // Liberar memoria
    Rectangulo_destruir(miRect);

    return 0;
}