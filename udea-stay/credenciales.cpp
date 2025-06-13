#include "Credenciales.h"
#include "utils.h"
#include <cstring>  // Para strcpy, strlen, strcmp
#include <iostream> // Para mostrar()

// Helper para duplicar C-strings de forma segura
char* duplicarCString(const char* s) {
    if (!s) return nullptr;
    char* nuevo = new char[strlen(s) + 1];
    strcpy(nuevo, s);
    return nuevo;
}

Credenciales::Credenciales(const char* user, const char* pass) {
    nombreUsuario = duplicarCString(user);


    password = duplicarCString(pass);
}

Credenciales::~Credenciales() {
    delete[] nombreUsuario;
    delete[] password;
}

Credenciales::Credenciales(const Credenciales& otra) {
    nombreUsuario = duplicarCString(otra.nombreUsuario);
    password = duplicarCString(otra.password);
}

Credenciales& Credenciales::operator=(const Credenciales& otra) {
    if (this == &otra) {
        return *this;
    }
    delete[] nombreUsuario;
    delete[] password;
    nombreUsuario = duplicarCString(otra.nombreUsuario);
    password = duplicarCString(otra.password);
    return *this;
}

bool Credenciales::verificarPassword(const char* passIntento) const {
    if (!password || !passIntento) {
        return (!password && !passIntento); // Ambos nulos se consideran "iguales" en este contexto simple
    }
    return strcmp(password, passIntento) == 0;
}

void Credenciales::mostrar() const {
    std::cout << "Usuario: " << (nombreUsuario ? nombreUsuario : "N/A") << std::endl;


}
