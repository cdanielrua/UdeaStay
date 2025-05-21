#ifndef HUESPED_H
#define HUESPED_H

//#include "Fecha.h"
//#include "Credenciales.h"
//#include "ListaPersonalizada.h"

// Forward declarations
class Alojamiento;
class Reserva;

class Huesped {
private:
    char* numeroDocumento;
    char* nombreCompleto;
    Credenciales credenciales;
    int antiguedadPlataformaMeses;
    float puntuacion;
    ListaPersonalizada<Reserva*>* reservasRealizadas; // Lista de punteros a Reserva

    // Helper (si no es global)
    // char* duplicarCString(const char* s) const;

public:
    // Constructores y Destructor
    Huesped(const char* doc = "", const char* nombre = "",
            const Credenciales& creds = Credenciales(),
            int antiguedad = 0, float punt = 0.0f);
    Huesped(const Huesped& otro); // Constructor de copia
    ~Huesped();

    // Operador de asignación
    Huesped& operator=(const Huesped& otro);

    // Getters
    const char* getNumeroDocumento() const;
    const char* getNombreCompleto() const;
    const Credenciales& getCredenciales() const;
    int getAntiguedad() const;
    float getPuntuacion() const;
    const ListaPersonalizada<Reserva*>* getReservasRealizadas() const;

    // Setters
    void setNombreCompleto(const char* nombre);
    void setPuntuacion(float nuevaPuntuacion);
    void setCredenciales(const Credenciales& creds);

    // Funcionalidades
    bool verificarPassword(const char* passIntento) const;

    // La funcionalidad de buscar alojamientos es compleja y probablemente la coordine SistemaUdeAStay,
    // pero el Huesped puede tener un registro de sus reservas.
    void agregarReferenciaReserva(Reserva* nuevaReserva); // El Huesped NO es dueño de la Reserva
    bool cancelarReferenciaReserva(const char* codigoReservacion); // Solo quita la referencia

    // Operación de despliegue
    void mostrarInformacion() const;
};

#endif // HUESPED_H
