#ifndef ALOJAMIENTO_H
#define ALOJAMIENTO_H

#include "Fecha.h"
#include "ListaPersonalizada.h"

// Forward declarations
class Anfitrion;
class Reserva;

class Alojamiento {
private:
    char* codigoIdentificador;
    char* nombre;
    Anfitrion* anfitrionResponsable; // Puntero al Anfitrion dueño (NO posee la memoria del Anfitrion)
    char* departamento;
    char* municipio;
    char* tipo; // e.g., "casa", "apartamento"
    char* direccion;
    float precioPorNoche;
    ListaPersonalizada<char*>* amenidades; // Lista dinámica de cadenas (amenidades)
    ListaPersonalizada<Reserva*>* referenciasAReservas; // Punteros a las reservas activas de este alojamiento

    // Helper (si no es global)
    // char* duplicarCString(const char* s) const;

public:
    Alojamiento(const char* cod = "", const char* nom = "", Anfitrion* anfitrion = nullptr,
                const char* dep = "", const char* mun = "", const char* tipoAloj = "",
                const char* dir = "", float precio = 0.0f);
    Alojamiento(const Alojamiento& otro);
    ~Alojamiento();

    Alojamiento& operator=(const Alojamiento& otro);

    // Getters
    const char* getCodigoIdentificador() const;
    const char* getNombre() const;
    Anfitrion* getAnfitrionResponsable() const; // Devuelve puntero, no const, si se permite modificarlo externamente (con cuidado)
        // o const Anfitrion* si es solo lectura.
    const char* getDepartamento() const;
    const char* getMunicipio() const;
    const char* getTipo() const;
    const char* getDireccion() const;
    float getPrecioPorNoche() const;
    const ListaPersonalizada<char*>* getAmenidades() const;
    const ListaPersonalizada<Reserva*>* getReferenciasAReservas() const;


    // Setters
    void setNombre(const char* nom);
    void setPrecioPorNoche(float precio);
    void setAnfitrionResponsable(Anfitrion* anfitrion); // Cambia la referencia

    // Funcionalidades
    bool estaDisponible(const Fecha& fechaInicio, int numNoches) const;
    void agregarReferenciaReserva(Reserva* reserva);
    bool eliminarReferenciaReserva(const char* codigoReserva);
    void agregarAmenidad(const char* amenidad);
    bool tieneAmenidad(const char* amenidad) const;

    void mostrarInformacionDetallada() const;
};

#endif // ALOJAMIENTO_H
