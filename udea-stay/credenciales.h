#ifndef CREDENCIALES_H
#define CREDENCIALES_H

class Credenciales {
public:
    char* nombreUsuario;
    char* password;

    Credenciales(const char* user = nullptr, const char* pass = nullptr);
    ~Credenciales();
    Credenciales(const Credenciales& otra);
    Credenciales& operator=(const Credenciales& otra);

    bool verificarPassword(const char* passIntento) const;
    void mostrar() const; // Para depuración
};

#endif // CREDENCIALES_H
