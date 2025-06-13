#ifndef ANFITRION_H
#define ANFITRION_H

#include "Fecha.h"        // Para tipos de Fecha en parámetros o retornos
#include "Credenciales.h" // Para el miembro credenciales
#include "ListaPersonalizada.h" // Para la lista de alojamientos

// Forward declarations para evitar dependencias circulares completas
class Alojamiento;
class Reserva;

class Anfitrion {
private:
    char* numeroDocumento;
    char* nombreCompleto;
    Credenciales credenciales; // Objeto Credenciales gestiona su propia memoria interna
    int antiguedadPlataformaMeses;
    float puntuacion;
    ListaPersonalizada<Alojamiento*>* alojamientosAdministrados; // Lista de punteros a Alojamiento

    // Helper para duplicar C-strings de forma segura (podría estar en un utils.h)
    char* duplicarCString(const char* s) const;

public:
    // Constructores y Destructor
    Anfitrion(const char* doc = "", const char* nombre = "",
              const Credenciales& creds = Credenciales(),
              int antiguedad = 0, float punt = 0.0f);
    Anfitrion(const Anfitrion& otro); // Constructor de copia
    ~Anfitrion();

    // Operador de asignación
    Anfitrion& operator=(const Anfitrion& otro);

    // Getters
    const char* getNumeroDocumento() const;
    const char* getNombreCompleto() const;
    const Credenciales& getCredenciales() const; // Devuelve const ref para evitar copia innecesaria
    int getAntiguedad() const;
    float getPuntuacion() const;
    const ListaPersonalizada<Alojamiento*>* getAlojamientosAdministrados() const;

    // Setters
    void setNombreCompleto(const char* nombre);
    void setPuntuacion(float nuevaPuntuacion);
    void setCredenciales(const Credenciales& creds);
    // Nota: Cambiar numeroDocumento o antiguedad usualmente no es una operación común post-creación.

    // Funcionalidades
    bool verificarPassword(const char* passIntento) const;
    void agregarAlojamiento(Alojamiento* nuevoAlojamiento); // Añade un puntero, no toma posesión de la memoria del Alojamiento
    bool eliminarAlojamiento(const char* codigoAlojamiento); // Elimina puntero de la lista




    ListaPersonalizada<Reserva*>* consultarReservacionesActivas(const Fecha& fechaInicio, const Fecha& fechaFin) const;



    bool anularReservacionEnAlojamiento(const char* codigoReservacion, const char* codigoAlojamiento);

    // Operación de despliegue
    void mostrarInformacion() const;




};

#endif // ANFITRION_H
