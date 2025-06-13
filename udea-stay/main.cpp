#include <iostream>
#include "SistemaUdeAStay.h" // Incluye la clase principal que orquesta todo.









int main() {
    // Mensaje de bienvenida opcional
    std::cout << "****************************************" << std::endl;
    std::cout << "* Bienvenido al Sistema UdeAStay  *" << std::endl;
    std::cout << "****************************************" << std::endl;
    std::cout << std::endl;

    // Crear el objeto principal del sistema.
    // El constructor de SistemaUdeAStay podría inicializar listas,
    // pero la carga de datos se hará explícitamente en iniciarSistema().
    SistemaUdeAStay sistema;

    // Iniciar el sistema. Este método contendrá el bucle principal del programa,
    // incluyendo la carga de datos, la presentación de menús y la interacción con el usuario,
    // y finalmente el guardado de datos.
    sistema.iniciarSistema();

    // Mensaje de despedida
    std::cout << std::endl;
    std::cout << "****************************************" << std::endl;
    std::cout << "* Gracias por usar UdeAStay. Adios!  *" << std::endl;
    std::cout << "****************************************" << std::endl;

    return 0; // Indica que el programa terminó exitosamente.
}
