//#include "Huesped.h"
//#include "Reserva.h"     // Para trabajar con Reserva*
//#include "Alojamiento.h" // Para el contexto de reserva (aunque no directamente usado aquí)
//#include "utils.h"
#include <iostream>   // Para mostrarInformacion
#include <cstring>    // Para strcpy, strlen, strcmp

// Asumiendo utils.h con duplicarCStringGlobal
// Si no, define aquí char* Huesped::duplicarCString(const char* s) const { ... }
// como se hizo en Anfitrion.cpp o incluye tu utilidad.
// Para este ejemplo, usaré la idea de que duplicarCStringGlobal existe:
char* duplicarCStringGlobal(const char* s); // Declaración si no está en un .h incluido globalmente

// Constructor
Huesped::Huesped(const char* doc, const char* nombre,
                 const Credenciales& creds,
                 int antiguedad, float punt)
    : credenciales(creds), antiguedadPlataformaMeses(antiguedad), puntuacion(punt) {
    numeroDocumento = duplicarCStringGlobal(doc);
    nombreCompleto = duplicarCStringGlobal(nombre);
    reservasRealizadas = new ListaPersonalizada<Reserva*>();
}

// Constructor de copia
Huesped::Huesped(const Huesped& otro)
    : credenciales(otro.credenciales),
    antiguedadPlataformaMeses(otro.antiguedadPlataformaMeses),
    puntuacion(otro.puntuacion) {
    numeroDocumento = duplicarCStringGlobal(otro.numeroDocumento);
    nombreCompleto = duplicarCStringGlobal(otro.nombreCompleto);
    reservasRealizadas = new ListaPersonalizada<Reserva*>(*(otro.reservasRealizadas)); // Copia la lista de punteros
}

// Destructor
Huesped::~Huesped() {
    delete[] numeroDocumento;
    delete[] nombreCompleto;
    // credenciales se destruye automáticamente
    // La lista reservasRealizadas almacena punteros a Reservas que son propiedad
    // de SistemaUdeAStay (en su lista histórica o una lista activa).
    // Por tanto, Huesped solo borra la lista de punteros, no las Reservas en sí.
    delete reservasRealizadas;
}

// Operador de asignación
Huesped& Huesped::operator=(const Huesped& otro) {
    if (this == &otro) {
        return *this;
    }

    delete[] numeroDocumento;
    delete[] nombreCompleto;
    // La lista de reservas anterior se borra antes de crear una nueva
    delete reservasRealizadas;

    antiguedadPlataformaMeses = otro.antiguedadPlataformaMeses;
    puntuacion = otro.puntuacion;
    credenciales = otro.credenciales; // Usa operator= de Credenciales

    numeroDocumento = duplicarCStringGlobal(otro.numeroDocumento);
    nombreCompleto = duplicarCStringGlobal(otro.nombreCompleto);
    reservasRealizadas = new ListaPersonalizada<Reserva*>(*(otro.reservasRealizadas));

    return *this;
}

// Getters
const char* Huesped::getNumeroDocumento() const { return numeroDocumento; }
const char* Huesped::getNombreCompleto() const { return nombreCompleto; }
const Credenciales& Huesped::getCredenciales() const { return credenciales; }
int Huesped::getAntiguedad() const { return antiguedadPlataformaMeses; }
float Huesped::getPuntuacion() const { return puntuacion; }
const ListaPersonalizada<Reserva*>* Huesped::getReservasRealizadas() const { return reservasRealizadas; }

// Setters
void Huesped::setNombreCompleto(const char* nombre) {
    delete[] nombreCompleto;
    nombreCompleto = duplicarCStringGlobal(nombre);
}

void Huesped::setPuntuacion(float nuevaPuntuacion) {
    if (nuevaPuntuacion >= 0.0f && nuevaPuntuacion <= 5.0f) {
        puntuacion = nuevaPuntuacion;
    }
}

void Huesped::setCredenciales(const Credenciales& creds) {
    credenciales = creds;
}

// Funcionalidades
bool Huesped::verificarPassword(const char* passIntento) const {
    return credenciales.verificarPassword(passIntento);
}

void Huesped::agregarReferenciaReserva(Reserva* nuevaReserva) {
    if (nuevaReserva && reservasRealizadas) {
        reservasRealizadas->agregarPuntero(nuevaReserva);
    }
}

bool Huesped::cancelarReferenciaReserva(const char* codigoReservacion) {
    if (!codigoReservacion || !reservasRealizadas) {
        return false;
    }
    for (int i = 0; i < reservasRealizadas->getTamano(); ++i) {
        Reserva* actual = reservasRealizadas->obtener(i);
        if (actual && strcmp(actual->getCodigoReserva(), codigoReservacion) == 0) {
            reservasRealizadas->eliminar(i);
            // No se hace 'delete actual;' aquí. SistemaUdeAStay es dueño de la Reserva.
            return true;
        }
    }
    return false; // No encontrada
}

// Operación de despliegue
void Huesped::mostrarInformacion() const {
    std::cout << "--- Informacion del Huesped ---" << std::endl;
    std::cout << "Documento: " << (numeroDocumento ? numeroDocumento : "N/A") << std::endl;
    std::cout << "Nombre: " << (nombreCompleto ? nombreCompleto : "N/A") << std::endl;
    std::cout << "Antiguedad: " << antiguedadPlataformaMeses << " meses" << std::endl;
    std::cout << "Puntuacion: " << puntuacion << "/5.0" << std::endl;
    if (reservasRealizadas && reservasRealizadas->getTamano() > 0) {
        std::cout << "Reservas Realizadas (" << reservasRealizadas->getTamano() << "):" << std::endl;
        for (int i = 0; i < reservasRealizadas->getTamano(); ++i) {
            Reserva* res = reservasRealizadas->obtener(i);
            if (res) {
                // Asumiendo que Reserva tiene un método para mostrar un resumen o su código
                std::cout << "  - Codigo Reserva: " << res->getCodigoReserva();
                if (res->getAlojamiento()) {
                    std::cout << ", Alojamiento: " << res->getAlojamiento()->getCodigoIdentificador(); // Asumiendo getCodigoIdentificador() en Alojamiento
                }
                std::cout << std::endl;
            }
        }
    } else {
        std::cout << "Reservas Realizadas: Ninguna" << std::endl;
    }
    std::cout << "-----------------------------" << std::endl;
}
