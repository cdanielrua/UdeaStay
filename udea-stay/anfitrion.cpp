#include "Anfitrion.h"
#include "Alojamiento.h" // Necesario para trabajar con Alojamiento* y sus métodos
#include "Reserva.h"     // Necesario para el tipo de retorno ListaPersonalizada<Reserva*>*
#include "utils.h"
#include <iostream>   // Para mostrarInformacion
#include <cstring>    // Para strcpy, strlen, strcmp

// Implementación del helper duplicarCString (si no está en un utils.h)
char* Anfitrion::duplicarCString(const char* s) const {
    if (!s) {
        char* emptyStr = new char[1];
        emptyStr[0] = '\0';
        return emptyStr;
    }
    char* nuevo = new char[strlen(s) + 1];
    strcpy(nuevo, s);
    return nuevo;
}

// Constructor
Anfitrion::Anfitrion(const char* doc, const char* nombre,
                     const Credenciales& creds,
                     int antiguedad, float punt)
    : credenciales(creds), antiguedadPlataformaMeses(antiguedad), puntuacion(punt) {
    numeroDocumento = duplicarCString(doc);
    nombreCompleto = duplicarCString(nombre);
    alojamientosAdministrados = new ListaPersonalizada<Alojamiento*>();
}

// Constructor de copia
Anfitrion::Anfitrion(const Anfitrion& otro)
    : credenciales(otro.credenciales), // Usa el constructor de copia de Credenciales
    antiguedadPlataformaMeses(otro.antiguedadPlataformaMeses),
    puntuacion(otro.puntuacion) {
    numeroDocumento = duplicarCString(otro.numeroDocumento);
    nombreCompleto = duplicarCString(otro.nombreCompleto);

    // Copia profunda de la lista de punteros a alojamientos (copia los punteros, no los objetos Alojamiento)
    alojamientosAdministrados = new ListaPersonalizada<Alojamiento*>(*(otro.alojamientosAdministrados));
}

// Destructor
Anfitrion::~Anfitrion() {
    delete[] numeroDocumento;
    delete[] nombreCompleto;
    // El objeto 'credenciales' se destruye automáticamente.

    // IMPORTANTE: Esta lista solo almacena punteros. La memoria de los objetos Alojamiento
    // a los que apuntan estos punteros DEBE ser gestionada por otra clase (ej. SistemaUdeAStay).
    // El destructor de ListaPersonalizada<Alojamiento*> borrará el array de punteros,
    // pero no los Alojamiento en sí.
    delete alojamientosAdministrados;
}

// Operador de asignación
Anfitrion& Anfitrion::operator=(const Anfitrion& otro) {
    if (this == &otro) {
        return *this; // Protección contra auto-asignación
    }

    // Liberar recursos antiguos
    delete[] numeroDocumento;
    delete[] nombreCompleto;
    // La asignación de 'credenciales' usará el operator= de Credenciales

    // Copiar datos escalares
    antiguedadPlataformaMeses = otro.antiguedadPlataformaMeses;
    puntuacion = otro.puntuacion;
    credenciales = otro.credenciales; // Usa el operator= de Credenciales

    // Copiar datos dinámicos
    numeroDocumento = duplicarCString(otro.numeroDocumento);
    nombreCompleto = duplicarCString(otro.nombreCompleto);

    // Manejar la lista de alojamientos
    delete alojamientosAdministrados; // Borra la lista antigua
    alojamientosAdministrados = new ListaPersonalizada<Alojamiento*>(*(otro.alojamientosAdministrados)); // Copia profunda de la lista

    return *this;
}

// Getters
const char* Anfitrion::getNumeroDocumento() const {
    return numeroDocumento;
}

const char* Anfitrion::getNombreCompleto() const {
    return nombreCompleto;
}

const Credenciales& Anfitrion::getCredenciales() const {
    return credenciales;
}

int Anfitrion::getAntiguedad() const {
    return antiguedadPlataformaMeses;
}

float Anfitrion::getPuntuacion() const {
    return puntuacion;
}

const ListaPersonalizada<Alojamiento*>* Anfitrion::getAlojamientosAdministrados() const {
    return alojamientosAdministrados;
}

// Setters
void Anfitrion::setNombreCompleto(const char* nombre) {
    delete[] nombreCompleto;
    nombreCompleto = duplicarCString(nombre);
}

void Anfitrion::setPuntuacion(float nuevaPuntuacion) {
    if (nuevaPuntuacion >= 0.0f && nuevaPuntuacion <= 5.0f) {
        puntuacion = nuevaPuntuacion;
    }
}

void Anfitrion::setCredenciales(const Credenciales& creds) {
    credenciales = creds; // Utiliza el operador= de Credenciales
}

// Funcionalidades
bool Anfitrion::verificarPassword(const char* passIntento) const {
    return credenciales.verificarPassword(passIntento);
}

void Anfitrion::agregarAlojamiento(Alojamiento* nuevoAlojamiento) {
    if (nuevoAlojamiento && alojamientosAdministrados) {
        // Opcional: verificar si ya existe para evitar duplicados
        alojamientosAdministrados->agregarPuntero(nuevoAlojamiento);
    }
}

bool Anfitrion::eliminarAlojamiento(const char* codigoAlojamiento) {
    if (!codigoAlojamiento || !alojamientosAdministrados) {
        return false;
    }
    for (int i = 0; i < alojamientosAdministrados->getTamano(); ++i) {
        Alojamiento* actual = alojamientosAdministrados->obtener(i);
        if (actual && strcmp(actual->getCodigoIdentificador(), codigoAlojamiento) == 0) {
            alojamientosAdministrados->eliminar(i);
            // Nota: No se hace 'delete actual;' aquí. La memoria del Alojamiento
            // es gestionada por SistemaUdeAStay. Solo se quita de esta lista.
            return true;
        }
    }
    return false; // No encontrado
}

ListaPersonalizada<Reserva*>* Anfitrion::consultarReservacionesActivas(const Fecha& fechaInicio, const Fecha& fechaFin) const {
    ListaPersonalizada<Reserva*>* resultado = new ListaPersonalizada<Reserva*>();
    if (!alojamientosAdministrados) return resultado; // Devuelve lista vacía

    for (int i = 0; i < alojamientosAdministrados->getTamano(); ++i) {
        Alojamiento* aloj = alojamientosAdministrados->obtener(i);
        if (aloj) {
            // Asumiendo que Alojamiento tiene un método para obtener sus reservas
            // o que podemos acceder a ellas y filtrarlas.
            // Esto es una simplificación. Alojamiento debería tener una forma de acceder
            // a sus ListaPersonalizada<Reserva*>* referenciasAReservas.
            const ListaPersonalizada<Reserva*>* reservasDelAloj = aloj->getReferenciasAReservas(); // Necesitarás este método en Alojamiento
            if (reservasDelAloj) {
                for (int j = 0; j < reservasDelAloj->getTamano(); ++j) {
                    Reserva* res = reservasDelAloj->obtener(j);
                    if (res) {
                        // Lógica para verificar si la reserva está activa y en el rango de fechas
                        // Fecha finReserva = res->calcularFechaSalida();
                        // if ( !(finReserva < fechaInicio || res->getFechaEntrada() > fechaFin) ) {
                        // Simplificado:
                        if (res->getFechaEntrada().anio >= fechaInicio.anio && res->getFechaEntrada().anio <= fechaFin.anio) { // Lógica de rango muy básica
                            resultado->agregarPuntero(res); // Se agregan punteros, no copias
                        }
                    }
                }
            }
        }
    }
    return resultado; // El llamador es responsable de borrar esta lista de resultados (pero no las Reservas en sí)
}

bool Anfitrion::anularReservacionEnAlojamiento(const char* codigoReservacion, const char* codigoAlojamiento) {
    // La lógica real de anulación (borrar el objeto Reserva, actualizar listas)
    // probablemente debería ser coordinada por SistemaUdeAStay.
    // Anfitrion solo verifica si la reserva pertenece a uno de sus alojamientos y solicita la anulación.
    if (!codigoReservacion || !codigoAlojamiento || !alojamientosAdministrados) return false;

    Alojamiento* alojamientoEncontrado = nullptr;
    for (int i = 0; i < alojamientosAdministrados->getTamano(); ++i) {
        Alojamiento* actual = alojamientosAdministrados->obtener(i);
        if (actual && strcmp(actual->getCodigoIdentificador(), codigoAlojamiento) == 0) {
            alojamientoEncontrado = actual;
            break;
        }
    }

    if (alojamientoEncontrado) {
        // Ahora, verificar si la reserva está en ese alojamiento y proceder
        // (esto usualmente llamaría a un método en Alojamiento o SistemaUdeAStay)
        // return alojamientoEncontrado->solicitarAnulacionReserva(codigoReservacion);
        std::cout << "INFO: Anfitrion " << nombreCompleto << " solicita anular reserva "
                  << codigoReservacion << " en alojamiento " << codigoAlojamiento << "." << std::endl;
        std::cout << "      (La logica final de anulacion estaria en SistemaUdeAStay o Alojamiento)" << std::endl;
        // Placeholder:
        // En un sistema real, buscarías la reserva en el alojamiento y la marcarías o eliminarías.
        // El Alojamiento podría tener un método:
        // bool exito = alojamientoEncontrado->anularReservaInterna(codigoReservacion);
        // O el SistemaUdeAStay:
        // bool exito = sistema->anularReservaGlobal(codigoReservacion, this);
        // if (exito) { /* actualizar estado si es necesario */ }
        // return exito;
        return true; // Simulación de éxito
    }
    return false; // Alojamiento no encontrado o no pertenece al anfitrión
}


// Operación de despliegue
void Anfitrion::mostrarInformacion() const {
    std::cout << "--- Informacion del Anfitrion ---" << std::endl;
    std::cout << "Documento: " << (numeroDocumento ? numeroDocumento : "N/A") << std::endl;
    std::cout << "Nombre: " << (nombreCompleto ? nombreCompleto : "N/A") << std::endl;
    // credenciales.mostrar(); // Podría mostrar el nombre de usuario si es relevante
    std::cout << "Antiguedad: " << antiguedadPlataformaMeses << " meses" << std::endl;
    std::cout << "Puntuacion: " << puntuacion << "/5.0" << std::endl;
    if (alojamientosAdministrados && alojamientosAdministrados->getTamano() > 0) {
        std::cout << "Alojamientos Administrados (" << alojamientosAdministrados->getTamano() << "):" << std::endl;
        for (int i = 0; i < alojamientosAdministrados->getTamano(); ++i) {
            Alojamiento* aloj = alojamientosAdministrados->obtener(i);
            if (aloj) {
                // Asumiendo que Alojamiento tiene un método para mostrar un resumen o su código/nombre
                std::cout << "  - Codigo: " << aloj->getCodigoIdentificador() << ", Nombre: " << aloj->getNombre() << std::endl;
            }
        }
    } else {
        std::cout << "Alojamientos Administrados: Ninguno" << std::endl;
    }
    std::cout << "---------------------------------" << std::endl;
}

/*
// Potencial sobrecarga de operadores
bool Anfitrion::operator==(const Anfitrion& otro) const {
    if (!numeroDocumento || !otro.numeroDocumento) return false;
    return strcmp(numeroDocumento, otro.numeroDocumento) == 0;
}

// friend std::ostream& operator<<(std::ostream& os, const Anfitrion& anfitrion) {
//     os << "Anfitrion: " << anfitrion.nombreCompleto << " (Doc: " << anfitrion.numeroDocumento << ")";
//     return os;
// }
*/
