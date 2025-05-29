#ifndef RESERVA_H
#define RESERVA_H

#include "Fecha.h"

// Forward declarations para evitar incluir .h completos y romper dependencias circulares
class Alojamiento;
class Huesped;

class Reserva {
private:
    char* codigoReserva;        // Gestionado automáticamente por el sistema
    Alojamiento* alojamiento;   // Puntero al Alojamiento reservado (NO posee la memoria del Alojamiento)
    Huesped* huesped;           // Puntero al Huesped que reserva (NO posee la memoria del Huesped)
    Fecha fechaEntrada;
    int duracionNoches;
    char* metodoPago;           // e.g., "PSE", "TCredito"
    Fecha fechaPago;
    float montoTotal;
    char* anotacionesHuesped;   // Hasta 1000 caracteres

    // Helper (si no es global)
    // char* duplicarCString(const char* s, int maxLength = 0) const;
    // maxLength para anotaciones

public:
    // Constructores y Destructor
    Reserva(const char* codRes = "", Alojamiento* aloj = nullptr, Huesped* hues = nullptr,
            const Fecha& fEnt = Fecha(), int durNoches = 0, const char* metPago = "",
            const Fecha& fPago = Fecha(), float monto = 0.0f, const char* anot = "");
    Reserva(const Reserva& otra); // Constructor de copia
    ~Reserva();

    // Operador de asignación
    Reserva& operator=(const Reserva& otra);

    // Getters
    const char* getCodigoReserva() const;
    Alojamiento* getAlojamiento() const; // Devuelve puntero para acceso
    Huesped* getHuesped() const;         // Devuelve puntero para acceso
    Fecha getFechaEntrada() const;
    int getDuracionNoches() const;
    const char* getMetodoPago() const;
    Fecha getFechaPago() const;
    float getMontoTotal() const;
    const char* getAnotacionesHuesped() const;

    // Setters (usar con precaución, especialmente para punteros y código de reserva)
    void setMetodoPago(const char* metPago);
    void setMontoTotal(float monto);
    void setAnotacionesHuesped(const char* anot);
    // Cambiar alojamiento, huésped, fechas o código post-creación usualmente implica
    // una lógica de negocio más compleja (ej. nueva reserva, anulación y recreación).

    // Funcionalidades
    Fecha calcularFechaSalida() const;
    char* generarComprobante() const; // Devuelve una cadena formateada (caller debe liberar)

    // Operación de despliegue
    void mostrarInformacion() const;
};

#endif // RESERVA_H
