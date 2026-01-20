#Librería
import os
import platform

#funciones
def borrarPantalla():
    oes = platform.system()
    if oes=="Windows":
        os.system("cls")
    else:
        os.system("clear")

def esperarTecla():
    input("\n\t\tIntroduce enter para seguir")

def descuento(data, cont):
    data=data-(data/10)
    cont+=1
    print(f"Por lo pronto se ha realizado el descuento un total de {cont} veces\n")
    if data > 1000:
        data, cont=descuento(data, cont)
        return data, cont
    else:
        return data, cont


#Programa
borrarPantalla()
cyc=True
while cyc:
    print("\n\t Programa de la promoción especial \n\n")
    cant=input('Introduce la cantidad del total completo o escriba "salir" para salir del programa: ').strip().lower()
    try:
        cant=float(cant)
    except:
        #caso sin número
        if cant=="salir":
            cyc=False
            borrarPantalla()
            print("\n\tGracias por usar el programa")
            esperarTecla()
        else:
            cyc=True
            borrarPantalla()
            print("Opción incorrecta, ingrese solo números positivos")
    else:
        if cant < 0:
            #caso negativo
            borrarPantalla()
            print("No se permiten números negativos")
        else:
            #caso correcto
            borrarPantalla()
            contador=0
            if cant > 1000:
                cant, contador=descuento(cant, contador)
            print(f"\n\t La cantidad total a pagar es de {cant} con un total de {contador} repeticiones!")
            esperarTecla()
            borrarPantalla()
