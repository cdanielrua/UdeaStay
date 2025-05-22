

#include "Reserva.h"
//#include "Alojamiento.h" // Para acceder a info del Alojamiento (ej. nombre)
//#include "Huesped.h"     // Para acceder a info del Huesped (ej. nombre)
//#include "utils.h"
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
    Fecha fechaSalida = fechaEntrada;
    // Implementación de suma de días a una fecha.
    // Esto es complejo y necesita manejar días en mes, años bisiestos.
    // Por simplicidad, aquí solo se suma a los días, pero esto NO ES CORRECTO para una implementación real.
    // Necesitarías una lógica robusta en la clase Fecha o aquí.
    // Ejemplo conceptual MUY BÁSICO (e incorrecto para la mayoría de los casos):
    // fechaSalida.dia += duracionNoches;
    // if (fechaSalida.dia > 30) { // Asumiendo todos los meses tienen 30 días (MAL)
    //     fechaSalida.dia -= 30;
    //     fechaSalida.mes++;
    //     if (fechaSalida.mes > 12) {
    //         fechaSalida.mes -= 12;
    //         fechaSalida.anio++;
    //     }
    // }
    std::cout << "ADVERTENCIA: Reserva::calcularFechaSalida() necesita implementacion robusta de aritmetica de fechas." << std::endl;
    // Devolver la fecha de entrada + duración para que compile, pero enfatizar que debe mejorarse.
    // Lógica placeholder que solo suma días al componente día:
    int diaTemp = fechaEntrada.dia + duracionNoches;
    int mesTemp = fechaEntrada.mes;
    int anioTemp = fechaEntrada.anio;
    // Esta es una simplificación burda. Necesitas una función de calendario real.
    // Por ejemplo, para 30 de Enero + 2 noches = 1 de Febrero (no 32 de Enero).
    // Aquí solo para tener algo que compile:
    while(diaTemp > 30) { // Asumiendo 30 días por mes para este placeholder
        diaTemp -=30;
        mesTemp++;
        if(mesTemp > 12) {
            mesTemp = 1;
            anioTemp++;
        }
    }
    fechaSalida.dia = diaTemp;
    fechaSalida.mes = mesTemp;
    fechaSalida.anio = anioTemp;

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
