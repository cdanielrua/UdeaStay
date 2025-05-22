// En un utils.h (o similar, para no repetir en cada .cpp)
#ifndef UTILS_H
#define UTILS_H
#include <cstring> // Para strlen, strcpy

inline char* duplicarCStringGlobal(const char* s) {
    if (!s) {
        char* emptyStr = new char[1];
        emptyStr[0] = '\0';
        return emptyStr;
    }
    char* nuevo = new char[strlen(s) + 1];
    strcpy(nuevo, s);
    return nuevo;
}
#endif //UTILS_H
