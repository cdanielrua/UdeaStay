#include "utils.h" // Incluye las declaraciones
#include <cstring> // Para strlen, strcpy que usa duplicarCStringGlobal

// Implementación de esAnioBisiesto
bool esAnioBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

// Implementación de diasEnMes
int diasEnMes(int mes, int anio) {
    if (mes < 1 || mes > 12) {
        return 0;
    }
    if (mes == 2) {
        return esAnioBisiesto(anio) ? 29 : 28;
    } else if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        return 30;
    } else {
        return 31;
    }
}

// Implementación de duplicarCStringGlobal
char* duplicarCStringGlobal(const char* s) {
    if (!s) {
        char* emptyStr = new char[1];
        emptyStr[0] = '\0';
        return emptyStr;
    }
    if (s[0] == '\0') {
        char* emptyStr = new char[1];
        emptyStr[0] = '\0';
        return emptyStr;
    }
    char* nuevo = new char[strlen(s) + 1];
    strcpy(nuevo, s);
    return nuevo;
}
