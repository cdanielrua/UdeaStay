#ifndef LISTAPERSONALIZADA_H
#define LISTAPERSONALIZADA_H

#include <iostream> // Para std::cerr en caso de error, o para operador<<
#include <stdexcept> // Para std::out_of_range (aunque podríamos evitar excepciones estándar también)

template <typename T>
class ListaPersonalizada {
private:
    T* elementos;       // Puntero al array dinámico de elementos
    int capacidad;      // Capacidad actual del array
    int tamano;         // Número actual de elementos en la lista

    void redimensionar(int nuevaCapacidad) {
        T* nuevosElementos = new T[nuevaCapacidad];
        for (int i = 0; i < tamano; ++i) {
            nuevosElementos[i] = elementos[i]; // Asume que T tiene operador=
        }
        delete[] elementos;
        elementos = nuevosElementos;
        capacidad = nuevaCapacidad;
    }

public:
    // Constructor
    ListaPersonalizada(int capInicial = 10) {
        capacidad = capInicial > 0 ? capInicial : 10;
        tamano = 0;
        elementos = new T[capacidad];
    }

    // Destructor
    ~ListaPersonalizada() {
        // Si T es un puntero, el destructor de ListaPersonalizada NO borra los objetos apuntados.
        // Eso debe hacerse externamente antes de destruir la lista o al limpiar la lista.
        delete[] elementos;
    }

    // Constructor de copia
    ListaPersonalizada(const ListaPersonalizada<T>& otra) {
        tamano = otra.tamano;
        capacidad = otra.capacidad;
        elementos = new T[capacidad];
        for (int i = 0; i < tamano; ++i) {
            elementos[i] = otra.elementos[i]; // Asume que T tiene operador=
        }
    }

    // Operador de asignación
    ListaPersonalizada<T>& operator=(const ListaPersonalizada<T>& otra) {
        if (this == &otra) {
            return *this;
        }
        delete[] elementos;

        tamano = otra.tamano;
        capacidad = otra.capacidad;
        elementos = new T[capacidad];
        for (int i = 0; i < tamano; ++i) {
            elementos[i] = otra.elementos[i]; // Asume que T tiene operador=
        }
        return *this;
    }

    void agregar(const T& elemento) {
        if (tamano == capacidad) {
            redimensionar(capacidad == 0 ? 1 : capacidad * 2);
        }
        elementos[tamano++] = elemento;
    }

    // Método para agregar punteros, común en este proyecto
    void agregarPuntero(T elementoPtr) { // T sería Tipo* en este caso
        if (tamano == capacidad) {
            redimensionar(capacidad == 0 ? 1 : capacidad * 2);
        }
        elementos[tamano++] = elementoPtr;
    }

    // Método para limpiar la lista de punteros (borrando los objetos apuntados)
    // Solo usar si T es un tipo puntero y la lista es dueña de los objetos.
    void limpiarPunteros() {
        for (int i = 0; i < tamano; ++i) {
            delete elementos[i]; // Asume que T es un puntero a un objeto borrable
            elementos[i] = nullptr; // Buena práctica
        }
        tamano = 0;
        // Podríamos también redimensionar a una capacidad inicial aquí si se desea.
    }


    T obtener(int indice) const {
        if (indice < 0 || indice >= tamano) {
            // Manejo de error, lanzar excepción o retornar un valor por defecto/error
            // Por simplicidad, podríamos imprimir un error y abortar o retornar un T()
            std::cerr << "Error: Indice fuera de rango." << std::endl;
            // Para un programa robusto, se usaría un manejo de errores mejor.
            throw std::out_of_range("Indice fuera de rango en ListaPersonalizada::obtener");
        }
        return elementos[indice];
    }

    bool eliminar(int indice) {
        if (indice < 0 || indice >= tamano) {
            return false; // O manejar error
        }
        // Si T es un puntero y la lista es dueña, considerar borrar el objeto apuntado: delete elementos[indice];
        for (int i = indice; i < tamano - 1; ++i) {
            elementos[i] = elementos[i + 1];
        }
        tamano--;
        // Opcional: reducir capacidad si tamano es mucho menor que capacidad
        return true;
    }

    int getTamano() const {
        return tamano;
    }

    bool estaVacia() const {
        return tamano == 0;
    }

    // Operador de acceso
    T& operator[](int indice) {
        if (indice < 0 || indice >= tamano) {
            std::cerr << "Error: Indice fuera de rango." << std::endl;
            throw std::out_of_range("Indice fuera de rango en ListaPersonalizada::operator[]");
        }
        return elementos[indice];
    }

    const T& operator[](int indice) const {
        if (indice < 0 || indice >= tamano) {
            std::cerr << "Error: Indice fuera de rango." << std::endl;
            throw std::out_of_range("Indice fuera de rango en ListaPersonalizada::operator[] const");
        }
        return elementos[indice];
    }
};

#endif // LISTAPERSONALIZADA_H
