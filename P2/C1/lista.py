# Ejemplo de lista enlazada simple para una lista de tareas

class Nodo:
    def __init__(self, dato):
        self.dato = dato      # Información que guarda el nodo
        self.siguiente = None # Puntero al siguiente nodo

class ListaEnlazada:
    def __init__(self):
        self.cabeza = None  # Primer nodo de la lista

    def agregar(self, dato):
        """Agrega un nuevo nodo al final de la lista."""
        nuevo_nodo = Nodo(dato)
        if not self.cabeza:
            self.cabeza = nuevo_nodo
            return
        actual = self.cabeza
        while actual.siguiente:
            actual = actual.siguiente
        actual.siguiente = nuevo_nodo

    def mostrar(self):
        """Muestra todos los elementos de la lista."""
        actual = self.cabeza
        while actual:
            print(actual.dato)
            actual = actual.siguiente

# Ejemplo de uso
tareas = ListaEnlazada()
tareas.agregar("Comprar pan")
tareas.agregar("Enviar correo")
tareas.agregar("Estudiar listas enlazadas")

print("Lista de tareas:")
tareas.mostrar()