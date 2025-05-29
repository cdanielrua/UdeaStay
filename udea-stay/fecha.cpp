#include "Fecha.h"
#include <cstdio>  // Para sprintf (o usa iostream y stringstream si estuvieran permitidos, pero no STL)
#include <cstring> // Para strlen, strcpy
#include <iostream> // Para mostrar()
#include "utils.h"
Fecha::Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {
    // Aquí podrían ir validaciones (ej. día entre 1-31, mes 1-12)
    // Por simplicidad, omitidas aquí.
}

int calcularDiaSemanaZeller(int d, int m, int a);

char* Fecha::toStringFormateado() const {
    const char* nombreMeses[] = {
        "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
        "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"
    };
    const char* nombreDiasSemana[] = { // Ajustado para el resultado de Zeller (0=Sábado, 1=Domingo, ..., 6=Viernes) o (0=Domingo, ..., 6=Sábado)
        // La implementación de Zeller que usaré devuelve: 0=Sábado, 1=Domingo, ..., 6=Viernes
        // Así que el array será: Sábado, Domingo, Lunes, Martes, Miércoles, Jueves, Viernes
        "Sábado", "Domingo", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes"
    };

    // Validar mes antes de usarlo como índice
    if (mes < 1 || mes > 12) {
        char* errorStr = new char[strlen("Fecha Invalida") + 1];
        strcpy(errorStr, "Fecha Invalida");
        return errorStr;
    }
    const char* mesStr = nombreMeses[mes - 1];

    // Calcular el día de la semana usando el Algoritmo de Zeller
    int diaSemanaIndex = calcularDiaSemanaZeller(dia, mes, anio); // Devuelve 0-6
    const char* diaSemanaStr = nombreDiasSemana[diaSemanaIndex];

    char buffer[128]; // Suficientemente grande para el formato
    // Formato: nombre Día, día "de" nombreMes "del" año
    sprintf(buffer, "%s, %d de %s del %d", diaSemanaStr, dia, mesStr, anio);

    char* resultado = new char[strlen(buffer) + 1];
    strcpy(resultado, buffer);
    return resultado; // El llamador es responsable de delete[] resultado;
}

// Implementación del Algoritmo de Zeller (Congruencia de Zeller)
// Devuelve: 0 para Sábado, 1 para Domingo, 2 para Lunes, ..., 6 para Viernes
// Nota: Para este algoritmo, Enero y Febrero se cuentan como los meses 13 y 14 del año anterior.
int calcularDiaSemanaZeller(int d, int m, int a) {
    if (m < 3) { // Enero (1) o Febrero (2)
        m += 12;
        a -= 1;
    }
    int k = a % 100; // Año del siglo
    int j = a / 100; // Siglo

    // Fórmula de Zeller
    int h = (d + ((13 * (m + 1)) / 5) + k + (k / 4) + (j / 4) + (5 * j)) % 7;
    // h = 0 para Sábado, 1 para Domingo, ..., 6 para Viernes.

    return h;
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

// Implementación de sumarDias (ejemplo conceptual básico):
/*
Fecha Fecha::sumarDias(int dias) const {
    Fecha nuevaFecha = *this;
    nuevaFecha.dia += dias;
    // Aquí iría la lógica para ajustar mes y año si nuevaFecha.dia excede los días del mes
    // Por ejemplo:
    // while (nuevaFecha.dia > DIAS_EN_MES[nuevaFecha.mes-1]) { // Asumiendo DIAS_EN_MES es un array
    //     nuevaFecha.dia -= DIAS_EN_MES[nuevaFecha.mes-1];
    //     nuevaFecha.mes++;
    //     if (nuevaFecha.mes > 12) {
    //         nuevaFecha.mes = 1;
    //         nuevaFecha.anio++;
    //     }
    // }
    return nuevaFecha;
}
*/
