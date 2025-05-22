#ifndef FECHA_H
#define FECHA_H

class Fecha {
public:
    int dia;
    int mes;
    int anio;

    Fecha(int d = 1, int m = 1, int a = 2000);
    char* toStringFormateado() const; // Caller debe liberar la memoria
    void mostrar() const;

    // Podrías necesitar operadores de comparación
    bool operator==(const Fecha& otra) const;
    bool operator!=(const Fecha& otra) const; // Útil tener el opuesto
    bool operator<(const Fecha& otra) const; // Para ordenamiento o rangos
    bool operator<=(const Fecha& otra) const;
    bool operator>(const Fecha& otra) const;
    bool operator>=(const Fecha& otra) const;
    // friend std::ostream& operator<<(std::ostream& os, const Fecha& f);
};

#endif // FECHA_H
