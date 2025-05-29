#include "Reserva.h"
#include "Alojamiento.h" // Para acceder a info del Alojamiento (ej. nombre)
#include "Huesped.h"     // Para acceder a info del Huesped (ej. nombre)
#include "utils.h"
#include <iostream>   // Para mostrarInformacion
#include <cstring>    // Para strcpy, strlen, strcmp
#include <cstdio>     // Para sprintf en generarComprobante

// Asumiendo utils.h con duplicarCStringGlobal
// Si no, define aquí char* duplicarCStringGlobal(const char* s, int maxLength = 0);
// donde maxLength puede ser usado para truncar o validar la longitud de anotacionesHuesped.
char* duplicarCStringGlobal(const char* s); // Ya definido/asumido previamente



// Helper específico para anotaciones con límite de longitud
char* duplicarCStringConLimite(const char* s, int maxLength) {
    if (!s) {
        char* emptyStr = new char[1];
        emptyStr[0] = '\0';
        return emptyStr;
    }
    int lenOriginal = strlen(s);
    int lenACopiar = (maxLength > 0 && lenOriginal > maxLength) ? maxLength : lenOriginal;

    char* nuevo = new char[lenACopiar + 1];
    strncpy(nuevo, s, lenACopiar);
    nuevo[lenACopiar] = '\0'; // Asegurar terminación nula
    return nuevo;
}


Reserva::Reserva(const char* codRes, Alojamiento* aloj, Huesped* hues,
                 const Fecha& fEnt, int durNoches, const char* metPago,
                 const Fecha& fPago, float monto, const char* anot)
    : alojamiento(aloj), huesped(hues), fechaEntrada(fEnt), duracionNoches(durNoches),
    fechaPago(fPago), montoTotal(monto) {
    codigoReserva = duplicarCStringGlobal(codRes);
    metodoPago = duplicarCStringGlobal(metPago);
    anotacionesHuesped = duplicarCStringConLimite(anot, 1000); // Limite de 1000 caracteres [cite: 226]
}

Reserva::Reserva(const Reserva& otra)
    : alojamiento(otra.alojamiento), // Copia puntero, no el objeto Alojamiento
    huesped(otra.huesped),         // Copia puntero, no el objeto Huesped
    fechaEntrada(otra.fechaEntrada),
    duracionNoches(otra.duracionNoches),
    fechaPago(otra.fechaPago),
    montoTotal(otra.montoTotal) {
    codigoReserva = duplicarCStringGlobal(otra.codigoReserva);
    metodoPago = duplicarCStringGlobal(otra.metodoPago);
    anotacionesHuesped = duplicarCStringGlobal(otra.anotacionesHuesped); // Asume que la longitud ya es correcta
}

Reserva::~Reserva() {
    delete[] codigoReserva;
    delete[] metodoPago;
    delete[] anotacionesHuesped;
    // Los punteros alojamiento y huesped no son propiedad de Reserva, no se borran aquí.
}

Reserva& Reserva::operator=(const Reserva& otra) {
    if (this == &otra) {
        return *this;
    }

    // Liberar recursos antiguos
    delete[] codigoReserva;
    delete[] metodoPago;
    delete[] anotacionesHuesped;

    // Copiar datos miembro a miembro
    alojamiento = otra.alojamiento; // Copia puntero
    huesped = otra.huesped;         // Copia puntero
    fechaEntrada = otra.fechaEntrada;
    duracionNoches = otra.duracionNoches;
    fechaPago = otra.fechaPago;
    montoTotal = otra.montoTotal;

    codigoReserva = duplicarCStringGlobal(otra.codigoReserva);
    metodoPago = duplicarCStringGlobal(otra.metodoPago);
    anotacionesHuesped = duplicarCStringGlobal(otra.anotacionesHuesped); // Asume que la longitud ya es correcta

    return *this;
}

// Getters
const char* Reserva::getCodigoReserva() const { return codigoReserva; }
Alojamiento* Reserva::getAlojamiento() const { return alojamiento; }
Huesped* Reserva::getHuesped() const { return huesped; }
Fecha Reserva::getFechaEntrada() const { return fechaEntrada; }
int Reserva::getDuracionNoches() const { return duracionNoches; }
const char* Reserva::getMetodoPago() const { return metodoPago; }
Fecha Reserva::getFechaPago() const { return fechaPago; }
float Reserva::getMontoTotal() const { return montoTotal; }
const char* Reserva::getAnotacionesHuesped() const { return anotacionesHuesped; }

// Setters (ejemplos)
void Reserva::setMetodoPago(const char* metPago) {
    delete[] metodoPago;
    metodoPago = duplicarCStringGlobal(metPago);
}

void Reserva::setMontoTotal(float monto) {
    if (monto >= 0) {
        montoTotal = monto;
    }
}

void Reserva::setAnotacionesHuesped(const char* anot) {
    delete[] anotacionesHuesped;
    anotacionesHuesped = duplicarCStringConLimite(anot, 1000); // Limite de 1000 caracteres [cite: 226]
}


// Funcionalidades

Fecha Reserva::calcularFechaSalida() const {
    Fecha fechaSalida = this->fechaEntrada; // Empezamos con la fecha de entrada
    int nochesASumar = this->duracionNoches;

    if (nochesASumar < 0) {
        // Manejar caso de duración negativa si es necesario, o asumir que siempre es >= 0
        // Por ahora, si es negativo, devolvemos la fecha de entrada.
        std::cerr << "Advertencia: Duracion de noches negativa en calcularFechaSalida()." << std::endl;
        return fechaSalida;
    }

    // Sumamos los días uno por uno o en bloques, manejando los desbordes de mes y año.
    // Una forma más directa es sumar todos los días y luego normalizar.
    fechaSalida.dia += nochesASumar;

    // Normalizar la fecha
    while (fechaSalida.dia > diasEnMes(fechaSalida.mes, fechaSalida.anio)) {
        fechaSalida.dia -= diasEnMes(fechaSalida.mes, fechaSalida.anio);
        fechaSalida.mes++;
        if (fechaSalida.mes > 12) {
            fechaSalida.mes = 1;
            fechaSalida.anio++;
        }
    }
    // Esta normalización podría necesitar un bucle si se suman muchos días
    // y se pasan varios meses/años. La forma anterior es más robusta para grandes sumas.

    // Una normalización más robusta si se suman muchos días de golpe:
    // Reiniciamos el cálculo para el bucle de normalización
    /*
    fechaSalida = this->fechaEntrada;
    for (int i = 0; i < nochesASumar; ++i) {
        fechaSalida.dia++;
        if (fechaSalida.dia > diasEnMes(fechaSalida.mes, fechaSalida.anio)) {
            fechaSalida.dia = 1;
            fechaSalida.mes++;
            if (fechaSalida.mes > 12) {
                fechaSalida.mes = 1;
                fechaSalida.anio++;
            }
        }
    }
    */
    // La primera forma (sumar todos los días y luego normalizar el mes/año) es generalmente más eficiente
    // si la normalización del mes/año se hace correctamente.

    // Asegurémonos que la normalización sea completa:
    // Si después de sumar `nochesASumar` al `dia`, este excede los días del mes actual,
    // debemos ajustar el mes y el día repetidamente.
    while (fechaSalida.dia > diasEnMes(fechaSalida.mes, fechaSalida.anio)) {
        fechaSalida.dia -= diasEnMes(fechaSalida.mes, fechaSalida.anio); // Restamos los días del mes actual
        fechaSalida.mes++;                                     // Avanzamos al siguiente mes
        if (fechaSalida.mes > 12) {                            // Si pasamos Diciembre
            fechaSalida.mes = 1;                               // Vamos a Enero
            fechaSalida.anio++;                                // Del siguiente año
        }
    }
    // Podría haber un caso donde el día sea 0 o negativo si duracionNoches es 0 y se resta
    // Pero el bucle anterior asume que dia es positivo.
    // Si duracionNoches es 0, fechaSalida es igual a fechaEntrada.

    return fechaSalida;
}
char* Reserva::generarComprobante() const {
    // "código de la reserva (gestionada automáticamente), nombre del usuario,
    // código del alojamiento, la fecha de inicio y de finalización.
    // Ambas fechas deben mostrarse en el formato: nombre Día, día "de" nombreMes "del" año." [cite: 243, 244, 245]

    char* nombreHuesped = (huesped && huesped->getNombreCompleto()) ? duplicarCStringGlobal(huesped->getNombreCompleto()) : duplicarCStringGlobal("N/A");
    char* codigoAloj = (alojamiento && alojamiento->getCodigoIdentificador()) ? duplicarCStringGlobal(alojamiento->getCodigoIdentificador()) : duplicarCStringGlobal("N/A");

    char* fechaInicioStr = fechaEntrada.toStringFormateado(); // Caller (este método) debe liberar
    Fecha fechaFin = calcularFechaSalida();
    char* fechaFinStr = fechaFin.toStringFormateado(); // Caller (este método) debe liberar

    char buffer[512]; // Estimación del tamaño
    sprintf(buffer, "--- Comprobante de Reserva ---\nCodigo Reserva: %s\nNombre Huesped: %s\nCodigo Alojamiento: %s\nFecha Inicio: %s\nFecha Fin: %s\n----------------------------",
            (codigoReserva ? codigoReserva : "N/A"),
            nombreHuesped,
            codigoAloj,
            fechaInicioStr,
            fechaFinStr);

    delete[] nombreHuesped;
    delete[] codigoAloj;
    delete[] fechaInicioStr;
    delete[] fechaFinStr;

    char* resultado = new char[strlen(buffer) + 1];
    strcpy(resultado, buffer);
    return resultado; // El llamador es responsable de hacer delete[] al resultado.
}

void Reserva::mostrarInformacion() const {
    std::cout << "--- Informacion de la Reserva ---" << std::endl;
    std::cout << "Codigo Reserva: " << (codigoReserva ? codigoReserva : "N/A") << std::endl;
    if (alojamiento) {
        std::cout << "Alojamiento (Codigo): " << (alojamiento->getCodigoIdentificador() ? alojamiento->getCodigoIdentificador() : "N/A") << std::endl;
    } else {
        std::cout << "Alojamiento: No asignado" << std::endl;
    }
    if (huesped) {
        std::cout << "Huesped (Nombre): " << (huesped->getNombreCompleto() ? huesped->getNombreCompleto() : "N/A")
        << " (Doc: " << (huesped->getNumeroDocumento() ? huesped->getNumeroDocumento() : "N/A") << ")" << std::endl;
    } else {
        std::cout << "Huesped: No asignado" << std::endl;
    }
    std::cout << "Fecha Entrada: "; fechaEntrada.mostrar(); std::cout << std::endl;
    std::cout << "Duracion: " << duracionNoches << " noches" << std::endl;
    Fecha fechaSalida = calcularFechaSalida();
    std::cout << "Fecha Salida (calculada): "; fechaSalida.mostrar(); std::cout << std::endl;
    std::cout << "Metodo de Pago: " << (metodoPago ? metodoPago : "N/A") << std::endl;
    std::cout << "Fecha Pago: "; fechaPago.mostrar(); std::cout << std::endl;
    std::cout << "Monto Total: $" << montoTotal << std::endl;
    std::cout << "Anotaciones: " << (anotacionesHuesped ? anotacionesHuesped : "Ninguna") << std::endl;
    std::cout << "---------------------------------" << std::endl;
}
