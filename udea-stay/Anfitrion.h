
#ifndef ANFITRION_H
#define ANFITRION_H

//#include "Fecha.h"
//#include "Credenciales.h"
//#include "ListaPersonalizada.h"

class Alojamiento;
class Reserva;

class Anfitrion {
private:
    char* numeroDocumento;
    char* nombreCompleto;
    Credenciales credenciales;
    int antiguedadPlataformaMeses;
    float puntuacion;
    ListaPersonalizada<Alojamiento*>* alojamientosAdministrados;

    char* duplicarCString(const char* s) const;

public:
    Anfitrion(const char* doc = "", const char* nombre = "",
              const Credenciales& creds = Credenciales(),
              int antiguedad = 0, float punt = 0.0f);
    Anfitrion(const Anfitrion& otro);
    ~Anfitrion();

    Anfitrion& operator=(const Anfitrion& otro);

    const char* getNumeroDocumento() const;
    const char* getNombreCompleto() const;
    const Credenciales& getCredenciales() const;
    int getAntiguedad() const;
    float getPuntuacion() const;
    const ListaPersonalizada<Alojamiento*>* getAlojamientosAdministrados() const;

    void setNombreCompleto(const char* nombre);
    void setPuntuacion(float nuevaPuntuacion);
    void setCredenciales(const Credenciales& creds);

    bool verificarPassword(const char* passIntento) const;
    void agregarAlojamiento(Alojamiento* nuevoAlojamiento);
    bool eliminarAlojamiento(const char* codigoAlojamiento);
    ListaPersonalizada<Reserva*>* consultarReservacionesActivas(const Fecha& fechaInicio, const Fecha& fechaFin) const;
    bool anularReservacionEnAlojamiento(const char* codigoReservacion, const char* codigoAlojamiento);

    void mostrarInformacion() const;
};

#endif
