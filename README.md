
# UdeAStay - Sistema de Gestión de Estadías Hogareñas

## Descripción
UdeAStay es un sistema de software desarrollado en C++ para la administración de un mercado de estadías hogareñas. Este proyecto se enfoca en modelar una situación del mundo real, permitiendo la gestión eficiente de alojamientos, reservaciones, anfitriones y huéspedes.

## Objetivos del Proyecto
* Desarrollar la capacidad de análisis y solución de problemas.
* Aplicar y verificar los conocimientos fundamentales de la programación orientada a objetos en C++: abstracción, encapsulación, relaciones, diseño de clases, y manejo de memoria.

## Funcionalidades Esenciales Implementadas
El sistema UdeAStay implementa las siguientes funcionalidades:

1.  **Carga y Actualización de Datos**:
    * Algoritmos internos para leer y actualizar los datos desde un almacenamiento permanente (archivos de texto).
    * Esta operación no es visible en el menú de usuario.

2.  **Ingreso a la Plataforma**:
    * Inicio de sesión según el perfil (huésped o anfitrión) utilizando credenciales personales.
    * Los datos para la verificación se recuperan del almacenamiento permanente.
    * No incluye funcionalidad de registro de nuevos usuarios; estos deben existir en los datos cargados.

3.  **Reservar Alojamiento (Exclusivo para Huéspedes)**:
    * **Búsqueda por Criterios**: Muestra alojamientos disponibles según fecha, municipio y cantidad de noches.
    * **Filtros Opcionales**: Por costo máximo por noche y/o puntuación mínima del anfitrión.
    * **Búsqueda por Código**: Permite buscar un alojamiento por su código identificador.
    * **Validaciones**:
        * El huésped no debe tener otras reservas que se solapen con las nuevas fechas.
        * El alojamiento debe estar disponible para la totalidad de la reservación.
    * **Comprobante de Confirmación**: Muestra código de reserva (auto-generado), nombre del usuario, código del alojamiento, fecha de inicio y finalización (formato: "nombre Día, día 'de' nombreMes 'del' año").

4.  **Anular Reservación**:
    * Disponible para el huésped que realizó la reserva o para el anfitrión del alojamiento reservado.
    * Elimina la reservación correspondiente al código indicado.

5.  **Consultar Reservaciones (Anfitrión)**:
    * Permite al anfitrión ver todas las reservaciones activas para sus alojamientos dentro de un rango de fechas específico.

6.  **Actualizar Histórico**:
    * Mueve reservaciones de fechas previas a una "fecha de corte" (ingresada por un anfitrión) al archivo histórico.
    * Prepara las estructuras de datos para permitir nuevas reservaciones en los próximos 12 meses desde la fecha de corte.

7.  **Medición del Consumo de Recursos**:
    * Automáticamente, al finalizar otras funcionalidades, muestra:
        * Cantidad de iteraciones estimadas para la tarea.
        * Memoria estimada consumida por los objetos creados.

## Tecnologías y Restricciones
* **Lenguaje**: C++.
* **Paradigma**: Programación Orientada a Objetos (POO).
* **Restricciones Clave**:
    * No uso de la STL (Standard Template Library).
    * Estructuras de datos y algoritmos de creación propia.
    * Uso obligatorio de memoria dinámica.
    * Modularidad de componentes.

## Diseño de Clases (Breve)
La solución se estructura en:
* **Clases Principales**: `SistemaUdeAStay` (orquestador), `Anfitrion`, `Huesped`, `Alojamiento`, `Reserva`.
* **Clases Auxiliares**: `ListaPersonalizada<T>` (lista dinámica propia), `Fecha` (manejo de fechas), `Credenciales` (datos de autenticación).
No se utiliza herencia para los roles de `Anfitrion` y `Huesped`. Las relaciones principales son de composición y asociación.

## Diseño de Archivos de Persistencia
Los datos persisten en archivos de texto plano (ej. `anfitriones.txt`, `alojamientos.txt`, etc.), uno por cada tipo de entidad principal.
* **Formato**: Cada línea representa un objeto.
* **Delimitador Principal**: Punto y coma (`;`) entre campos.
* **Sub-delimitador**: Barra vertical (`|`) para listas dentro de un campo (ej. amenidades).
* **Enlaces**: Se usan IDs para representar relaciones entre objetos.
* **Fechas**: Se guardan como día, mes y año en campos separados.

## Compilación y Ejecución (General)
1.  **Compilación**:
    * Usar un compilador de C++ (ej. g++).
    * Compilar todos los archivos `.cpp` juntos:
        ```bash
        g++ main.cpp SistemaUdeAStay.cpp Anfitrion.cpp Huesped.cpp Alojamiento.cpp Reserva.cpp Fecha.cpp Credenciales.cpp utils.cpp -o UdeAStayApp
        ```
    * (Ajustar si se usa Makefile o una estructura de proyecto diferente).

2.  **Archivos de Datos**:
    * Asegurar que los archivos `.txt` (ej. `anfitriones.txt`) existan en el directorio del ejecutable o donde el programa espere encontrarlos.
    * Deben seguir el formato diseñado.

3.  **Ejecución**:
    ```bash
    ./UdeAStayApp
    ```
    * Seguir las instrucciones del menú en consola.

## Autores
* Daniel Rúa
* Tomás Mesa

## Contexto Académico
Proyecto desarrollado para la asignatura Informática II (Desafío II), Semestre 2025-1, Facultad de Ingeniería, Universidad de Antioquia.