#include "Fecha.h"
#include <cstdio>  // Para sprintf (o usa iostream y stringstream si estuvieran permitidos, pero no STL)
#include <cstring> // Para strlen, strcpy
#include <iostream> // Para mostrar()

Fecha::Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {
    // Aquí podrían ir validaciones (ej. día entre 1-31, mes 1-12)
    // Por simplicidad, omitidas aquí.
}

char* Fecha::toStringFormateado() const {

    const char* nombreMeses[] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
                                 "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
    const char* nombreDiasSemana[] = {"Domingo", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado"};


    const char* diaSemanaStr = "DiaPlaceholder";
    const char* mesStr = (mes >= 1 && mes <= 12) ? nombreMeses[mes - 1] : "MesInvalido";

    char buffer[128]; // Suficientemente grande para el formato
    // Formato: nombre Día, día "de" nombreMes "del" año
    sprintf(buffer, "%s, %d de %s del %d", diaSemanaStr, dia, mesStr, anio);

    char* resultado = new char[strlen(buffer) + 1];
    strcpy(resultado, buffer);
    return resultado; // Caller es responsable de delete[] resultado;
}

void Fecha::mostrar() const {
    // Formato simple para depuración o uso interno
    std::cout << dia << "/" << mes << "/" << anio;
}

bool Fecha::operator==(const Fecha& otra) const {
    return dia == otra.dia && mes == otra.mes && anio == otra.anio;
}

bool Fecha::operator!=(const Fecha& otra) const {
    return !(*this == otra); // Reutiliza operator==
}

bool Fecha::operator<(const Fecha& otra) const {
    if (anio < otra.anio) return true;
    if (anio > otra.anio) return false;
    if (mes < otra.mes) return true;
    if (mes > otra.mes) return false;
    return dia < otra.dia;
}

bool Fecha::operator<=(const Fecha& otra) const {
    return (*this < otra) || (*this == otra);
}

bool Fecha::operator>(const Fecha& otra) const {
    return !(*this <= otra); // Reutiliza operadores ya definidos
}

bool Fecha::operator>=(const Fecha& otra) const {
    return !(*this < otra); // Reutiliza operadores ya definidos
}

