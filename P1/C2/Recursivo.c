//Librerías
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <stdlib.h>
#include <errno.h>

//declaración de una lista (porque wn-)
typedef struct{
    float data;
    int cont;
} lista;

//Funciones
void borrarPantalla(){
    #ifdef _WIN32
        const char *sys = "cls";
        system(sys);
    #else
        const char *sys = "clear";
        system(sys);
    #endif
}

void esperarTecla(){
    printf("\n\t\t Oprima enter para continuar");
    #ifdef _WIN32
        getch();
    #else
        getch_linux();
    #endif
}

void descuento(float data, int cont, float *dat, int *con){
    data = data - (data / 10);
    cont += 1;
    printf("Por lo pronto se ha realizado el descuento un total de %d veces\n\n", cont);
    if (data > 1000){
        descuento(data, cont, dat, con);
    } else {
        *dat = data;
        *con = cont;
    }
}

int strFloat(const char *str, float *resultado){
    char *endptr;
    errno = 0;

    float valor = strtof(str, &endptr);

    // 1. No se pudo convertir nada
    if (endptr == str || errno == ERANGE || *endptr != '\0')  {
        return -1;
    } else {
        *resultado = valor;
        return 0;
    }
}

//Programa
int main() {
    borrarPantalla();
    int cyc=1;
    while (cyc==1){
        //Menú inicial
        char opc[50];
        float cant;
        printf("\n\t Programa de la promocion especial \n\n");
        printf("Introduce la cantidad del total completo o escribe \"salir\" para salir del programa: ");
        scanf("%s", opc);
        
        int estado = strFloat(opc, &cant);
        switch (estado) {
            case 0:
                if (cant < 0){
                    //caso negativo
                    borrarPantalla();
                    printf("No se permiten numeros negativos");
                } else {
                    //caso correcto
                    borrarPantalla();
                    int contador = 0;
                    int cont_resultado;
                    float resultado;
                    if (cant > 1000) {
                        descuento(cant, contador, &resultado, &cont_resultado);
                    } else {
                        cont_resultado = contador;
                        resultado = cant;
                    }
                    printf("La cantidad total a pagar es de %.2f con un total de %d repeticiones!", resultado, cont_resultado);
                    esperarTecla();
                    borrarPantalla();
                }
            case -1:
                if (strcmp(opc, "salir") == 0) {
                    //salida
                    cyc=0;
                    borrarPantalla();
                    printf("\n\tGracias por usar el programa");
                    esperarTecla();
                } else {
                    //caso sin número
                    cyc=1;
                    borrarPantalla();
                    printf("Opcion incorrecta, ingrese solo numeros positivos");
                }
        }
    }
    return 0;
}

// cd P1/Act2
// gcc Recursivo.c -o Recursivo
// ./Recursivo