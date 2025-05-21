
#include "Anfitrion.h"
/*
#include "Alojamiento.h"
#include "Reserva.h"
#include "utils.h"
*/
#include <iostream>
#include <cstring>

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

Anfitrion::Anfitrion(const char* doc, const char* nombre,
                     const Credenciales& creds,
                     int antiguedad, float punt)
    : credenciales(creds), antiguedadPlataformaMeses(antiguedad), puntuacion(punt) {
    numeroDocumento = duplicarCString(doc);
    nombreCompleto = duplicarCString(nombre);
    alojamientosAdministrados = new ListaPersonalizada<Alojamiento*>();
}

Anfitrion::Anfitrion(const Anfitrion& otro)
    : credenciales(otro.credenciales),
    antiguedadPlataformaMeses(otro.antiguedadPlataformaMeses),
    puntuacion(otro.puntuacion) {
    numeroDocumento = duplicarCString(otro.numeroDocumento);
    nombreCompleto = duplicarCString(otro.nombreCompleto);
    alojamientosAdministrados = new ListaPersonalizada<Alojamiento*>(*(otro.alojamientosAdministrados));
}

Anfitrion::~Anfitrion() {
    delete[] numeroDocumento;
    delete[] nombreCompleto;
    delete alojamientosAdministrados;
}

Anfitrion& Anfitrion::operator=(const Anfitrion& otro) {
    if (this == &otro) {
        return *this;
    }
    delete[] numeroDocumento;
    delete[] nombreCompleto;
    antiguedadPlataformaMeses = otro.antiguedadPlataformaMeses;
    puntuacion = otro.puntuacion;
    credenciales = otro.credenciales;
    numeroDocumento = duplicarCString(otro.numeroDocumento);
    nombreCompleto = duplicarCString(otro.nombreCompleto);
    delete alojamientosAdministrados;
    alojamientosAdministrados = new ListaPersonalizada<Alojamiento*>(*(otro.alojamientosAdministrados));
    return *this;
}

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
    credenciales = creds;
}

bool Anfitrion::verificarPassword(const char* passIntento) const {
    return credenciales.verificarPassword(passIntento);
}

void Anfitrion::agregarAlojamiento(Alojamiento* nuevoAlojamiento) {
    if (nuevoAlojamiento && alojamientosAdministrados) {
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
            return true;
        }
    }
    return false;
}

ListaPersonalizada<Reserva*>* Anfitrion::consultarReservacionesActivas(const Fecha& fechaInicio, const Fecha& fechaFin) const {
    ListaPersonalizada<Reserva*>* resultado = new ListaPersonalizada<Reserva*>();
    if (!alojamientosAdministrados) return resultado;
    for (int i = 0; i < alojamientosAdministrados->getTamano(); ++i) {
        Alojamiento* aloj = alojamientosAdministrados->obtener(i);
        if (aloj) {
            const ListaPersonalizada<Reserva*>* reservasDelAloj = aloj->getReferenciasAReservas();
            if (reservasDelAloj) {
                for (int j = 0; j < reservasDelAloj->getTamano(); ++j) {
                    Reserva* res = reservasDelAloj->obtener(j);
                    if (res) {
                        if (res->getFechaEntrada().anio >= fechaInicio.anio && res->getFechaEntrada().anio <= fechaFin.anio) {
                            resultado->agregarPuntero(res);
                        }
                    }
                }
            }
        }
    }
    return resultado;
}

bool Anfitrion::anularReservacionEnAlojamiento(const char* codigoReservacion, const char* codigoAlojamiento) {
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
        std::cout << "INFO: Anfitrion " << nombreCompleto << " solicita anular reserva "
                  << codigoReservacion << " en alojamiento " << codigoAlojamiento << "." << std::endl;
        std::cout << "      (La logica final de anulacion estaria en SistemaUdeAStay o Alojamiento)" << std::endl;
        return true;
    }
    return false;
}

void Anfitrion::mostrarInformacion() const {
    std::cout << "--- Informacion del Anfitrion ---" << std::endl;
    std::cout << "Documento: " << (numeroDocumento ? numeroDocumento : "N/A") << std::endl;
    std::cout << "Nombre: " << (nombreCompleto ? nombreCompleto : "N/A") << std::endl;
    std::cout << "Antiguedad: " << antiguedadPlataformaMeses << " meses" << std::endl;
    std::cout << "Puntuacion: " << puntuacion << "/5.0" << std::endl;
    if (alojamientosAdministrados && alojamientosAdministrados->getTamano() > 0) {
        std::cout << "Alojamientos Administrados (" << alojamientosAdministrados->getTamano() << "):" << std::endl;
        for (int i = 0; i < alojamientosAdministrados->getTamano(); ++i) {
            Alojamiento* aloj = alojamientosAdministrados->obtener(i);
            if (aloj) {
                std::cout << "  - Codigo: " << aloj->getCodigoIdentificador() << ", Nombre: " << aloj->getNombre() << std::endl;
            }
        }
    } else {
        std::cout << "Alojamientos Administrados: Ninguno" << std::endl;
    }
    std::cout << "---------------------------------" << std::endl;
}
