#include "Alojamiento.h"
#include "Anfitrion.h" // Para Anfitrion*
#include "Reserva.h"   // Para Reserva*
#include "utils.h"
#include <iostream>
#include <cstring>

// Asumiendo utils.h con duplicarCStringGlobal
char* duplicarCStringGlobal(const char* s);

Alojamiento::Alojamiento(const char* cod, const char* nom, Anfitrion* anfitrion,
                         const char* dep, const char* mun, const char* tipoAloj,
                         const char* dir, float precio)
    : anfitrionResponsable(anfitrion), precioPorNoche(precio) {
    codigoIdentificador = duplicarCStringGlobal(cod);
    nombre = duplicarCStringGlobal(nom);
    departamento = duplicarCStringGlobal(dep);
    municipio = duplicarCStringGlobal(mun);
    tipo = duplicarCStringGlobal(tipoAloj);
    direccion = duplicarCStringGlobal(dir);
    amenidades = new ListaPersonalizada<char*>();
    referenciasAReservas = new ListaPersonalizada<Reserva*>();
}

Alojamiento::Alojamiento(const Alojamiento& otro)
    : anfitrionResponsable(otro.anfitrionResponsable), // Copia el puntero, no el objeto Anfitrion
    precioPorNoche(otro.precioPorNoche) {
    codigoIdentificador = duplicarCStringGlobal(otro.codigoIdentificador);
    nombre = duplicarCStringGlobal(otro.nombre);
    departamento = duplicarCStringGlobal(otro.departamento);
    municipio = duplicarCStringGlobal(otro.municipio);
    tipo = duplicarCStringGlobal(otro.tipo);
    direccion = duplicarCStringGlobal(otro.direccion);

    amenidades = new ListaPersonalizada<char*>();
    for (int i = 0; i < otro.amenidades->getTamano(); ++i) {
        amenidades->agregarPuntero(duplicarCStringGlobal(otro.amenidades->obtener(i)));
    }

    referenciasAReservas = new ListaPersonalizada<Reserva*>(*(otro.referenciasAReservas)); // Copia la lista de punteros a Reserva
}

Alojamiento::~Alojamiento() {
    delete[] codigoIdentificador;
    delete[] nombre;
    delete[] departamento;
    delete[] municipio;
    delete[] tipo;
    delete[] direccion;

    // Borrar las cadenas de las amenidades
    if (amenidades) {
        for (int i = 0; i < amenidades->getTamano(); ++i) {
            delete[] amenidades->obtener(i);
        }
        delete amenidades;
    }

    // La lista de referencias a reservas solo contiene punteros.
    // Los objetos Reserva son gestionados por SistemaUdeAStay.
    delete referenciasAReservas;
    // anfitrionResponsable es un puntero de observación, no se borra aquí.
}

Alojamiento& Alojamiento::operator=(const Alojamiento& otro) {
    if (this == &otro) return *this;

    delete[] codigoIdentificador;
    delete[] nombre;
    delete[] departamento;
    delete[] municipio;
    delete[] tipo;
    delete[] direccion;
    if (amenidades) {
        for (int i = 0; i < amenidades->getTamano(); ++i) {
            delete[] amenidades->obtener(i);
        }
        delete amenidades;
    }
    delete referenciasAReservas;

    anfitrionResponsable = otro.anfitrionResponsable; // Copia puntero
    precioPorNoche = otro.precioPorNoche;

    codigoIdentificador = duplicarCStringGlobal(otro.codigoIdentificador);
    nombre = duplicarCStringGlobal(otro.nombre);
    departamento = duplicarCStringGlobal(otro.departamento);
    municipio = duplicarCStringGlobal(otro.municipio);
    tipo = duplicarCStringGlobal(otro.tipo);
    direccion = duplicarCStringGlobal(otro.direccion);

    amenidades = new ListaPersonalizada<char*>();
    for (int i = 0; i < otro.amenidades->getTamano(); ++i) {
        amenidades->agregarPuntero(duplicarCStringGlobal(otro.amenidades->obtener(i)));
    }
    referenciasAReservas = new ListaPersonalizada<Reserva*>(*(otro.referenciasAReservas));

    return *this;
}

// Getters
const char* Alojamiento::getCodigoIdentificador() const { return codigoIdentificador; }
const char* Alojamiento::getNombre() const { return nombre; }
Anfitrion* Alojamiento::getAnfitrionResponsable() const { return anfitrionResponsable; }
const char* Alojamiento::getDepartamento() const { return departamento; }
const char* Alojamiento::getMunicipio() const { return municipio; }
const char* Alojamiento::getTipo() const { return tipo; }
const char* Alojamiento::getDireccion() const { return direccion; }
float Alojamiento::getPrecioPorNoche() const { return precioPorNoche; }
const ListaPersonalizada<char*>* Alojamiento::getAmenidades() const { return amenidades; }
const ListaPersonalizada<Reserva*>* Alojamiento::getReferenciasAReservas() const { return referenciasAReservas; }


// Setters
void Alojamiento::setNombre(const char* nom) {
    delete[] nombre;
    nombre = duplicarCStringGlobal(nom);
}
void Alojamiento::setPrecioPorNoche(float precio) {
    if (precio >= 0) precioPorNoche = precio;
}
void Alojamiento::setAnfitrionResponsable(Anfitrion* anfitrion) {
    anfitrionResponsable = anfitrion; // Asume que la gestión de vida del Anfitrion es externa
}

// Funcionalidades
// ... (otros includes y código de Alojamiento.cpp) ...

bool Alojamiento::estaDisponible(const Fecha& fechaInicioDeseada, int numNochesDeseadas) const {
    if (!referenciasAReservas || numNochesDeseadas <= 0) {
        // Si no hay reservas y se piden 0 o menos noches, ¿está disponible?
        // Asumamos que se requiere al menos 1 noche.
        // Si no hay reservas y se pide al menos 1 noche, está disponible.
        return numNochesDeseadas > 0;
    }








    for (int i = 0; i < referenciasAReservas->getTamano(); ++i) {
        Reserva* reservaExistente = referenciasAReservas->obtener(i);
        if (reservaExistente) {
            Fecha inicioExistente = reservaExistente->getFechaEntrada();
            Fecha finExistenteCalculada = reservaExistente->calcularFechaSalida(); // 'finExistente' del error era local, esta es de la reserva.

            // Lógica de solapamiento de rangos: [A, B] y [C, D] se solapan si A < D y C < B.
            // [fechaInicioDeseada, fechaFinDeseadaCalculada] vs [inicioExistente, finExistenteCalculada]

            // Para que este Alojamiento NO esté disponible, la nueva reserva (fechaInicioDeseada hasta fechaFinDeseadaCalculada)
            // debe solaparse con alguna reservaExistente (inicioExistente hasta finExistenteCalculada).

            // Necesitamos calcular correctamente fechaFinDeseadaCalculada
            Fecha fechaFinDeseadaCalculada = fechaInicioDeseada; // Iniciar copia
            // Aquí va la lógica correcta para sumar numNochesDeseadas a fechaInicioDeseada
            // Ejemplo (necesita implementación robusta en Fecha o aquí):
            int diaTemp = fechaFinDeseadaCalculada.dia + numNochesDeseadas;
            int mesTemp = fechaFinDeseadaCalculada.mes;
            int anioTemp = fechaFinDeseadaCalculada.anio;
            // Esta es una simplificación burda. Necesitas una función de calendario real.
            while(diaTemp > 30) { // Asumiendo 30 días por mes para este placeholder
                diaTemp -=30;
                mesTemp++;
                if(mesTemp > 12) {
                    mesTemp = 1;
                    anioTemp++;
                }
            }
            fechaFinDeseadaCalculada.dia = diaTemp;
            fechaFinDeseadaCalculada.mes = mesTemp;
            fechaFinDeseadaCalculada.anio = anioTemp;


            // Comprobación de solapamiento:
            // No se solapan si el nuevo rango termina antes de que el existente comience O
            // si el nuevo rango comienza después de que el existente termine.
            // Por lo tanto, se solapan si NO se cumple esa condición.
            bool noSolapamiento = (fechaFinDeseadaCalculada < inicioExistente || fechaFinDeseadaCalculada == inicioExistente) || // Nueva termina antes o justo cuando empieza la existente
                                  (fechaInicioDeseada > finExistenteCalculada || fechaInicioDeseada == finExistenteCalculada);  // Nueva empieza después o justo cuando termina la existente

            if (!noSolapamiento) {
                return false; // Hay solapamiento, no está disponible
            }
        }
    }
    return true; // Si no se encontraron solapamientos
}

// ... (resto de Alojamiento.cpp) ...

void Alojamiento::agregarReferenciaReserva(Reserva* reserva) {
    if (reserva && referenciasAReservas) {
        referenciasAReservas->agregarPuntero(reserva);
    }
}

bool Alojamiento::eliminarReferenciaReserva(const char* codigoReserva) {
    if (!codigoReserva || !referenciasAReservas) return false;
    for (int i = 0; i < referenciasAReservas->getTamano(); ++i) {
        Reserva* res = referenciasAReservas->obtener(i);
        if (res && strcmp(res->getCodigoReserva(), codigoReserva) == 0) {
            referenciasAReservas->eliminar(i);
            return true;
        }
    }
    return false;
}

void Alojamiento::agregarAmenidad(const char* amenidad) {
    if (amenidad && amenidades) {
        amenidades->agregarPuntero(duplicarCStringGlobal(amenidad));
    }
}

bool Alojamiento::tieneAmenidad(const char* amenidad) const {
    if (!amenidad || !amenidades) return false;
    for (int i = 0; i < amenidades->getTamano(); ++i) {
        if (strcmp(amenidades->obtener(i), amenidad) == 0) {
            return true;
        }
    }
    return false;
}

void Alojamiento::mostrarInformacionDetallada() const {
    std::cout << "--- Detalles del Alojamiento ---" << std::endl;
    std::cout << "Codigo: " << (codigoIdentificador ? codigoIdentificador : "N/A") << std::endl;
    std::cout << "Nombre: " << (nombre ? nombre : "N/A") << std::endl;
    if (anfitrionResponsable && anfitrionResponsable->getNombreCompleto()) {
        std::cout << "Anfitrion: " << anfitrionResponsable->getNombreCompleto()
        << " (Doc: " << anfitrionResponsable->getNumeroDocumento() << ")" << std::endl;
    } else {
        std::cout << "Anfitrion: No asignado" << std::endl;
    }
    std::cout << "Ubicacion: " << (municipio ? municipio : "N/A") << ", " << (departamento ? departamento : "N/A") << std::endl;
    std::cout << "Direccion: " << (direccion ? direccion : "N/A") << std::endl;
    std::cout << "Tipo: " << (tipo ? tipo : "N/A") << std::endl;
    std::cout << "Precio por Noche: $" << precioPorNoche << std::endl;
    std::cout << "Amenidades: ";
    if (amenidades && amenidades->getTamano() > 0) {
        for (int i = 0; i < amenidades->getTamano(); ++i) {
            std::cout << amenidades->obtener(i) << (i == amenidades->getTamano() - 1 ? "" : ", ");
        }
        std::cout << std::endl;
    } else {
        std::cout << "Ninguna registrada." << std::endl;
    }
    std::cout << "Reservas Activas Conocidas: " << (referenciasAReservas ? referenciasAReservas->getTamano() : 0) << std::endl;
    // Mostrar detalles de reservas podría ser demasiado verboso aquí.
    std::cout << "------------------------------" << std::endl;
}
