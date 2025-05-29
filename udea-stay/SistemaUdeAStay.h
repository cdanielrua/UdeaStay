#ifndef SISTEMAUDEASTAY_H
#define SISTEMAUDEASTAY_H

#include "ListaPersonalizada.h"
#include "Fecha.h" // Para la fecha de corte y otros usos

// Forward declarations para no incluir todos los headers aquí si no es estrictamente necesario
// Sin embargo, para los tipos de las listas, los headers completos son usualmente incluidos.
#include "Anfitrion.h"
#include "Huesped.h"
#include "Alojamiento.h"
#include "Reserva.h"

// Constantes para nombres de archivos (ejemplo)
const char* const ARCHIVO_ANFITRIONES = "anfitriones.txt";
const char* const ARCHIVO_HUESPEDES = "huespedes.txt";
const char* const ARCHIVO_ALOJAMIENTOS = "alojamientos.txt";
const char* const ARCHIVO_RESERVAS_VIGENTES = "reservas_vigentes.txt"; // O todas las reservas activas
const char* const ARCHIVO_RESERVAS_HISTORICAS = "reservas_historicas.txt";


class SistemaUdeAStay {
private:
    ListaPersonalizada<Anfitrion*>* listaAnfitriones;
    ListaPersonalizada<Huesped*>* listaHuespedes;
    ListaPersonalizada<Alojamiento*>* listaAlojamientos;
    // Podríamos tener una lista para reservas vigentes si no están solo en Alojamiento/Huesped
    ListaPersonalizada<Reserva*>* listaReservasActivas;
    ListaPersonalizada<Reserva*>* historicoReservas; // Reservas pasadas

    // Para medición de recursos
    long long contadorIteracionesGlobal; // Un contador muy simple
    size_t memoriaEstimadaGlobal;    // Estimación simple

    // Métodos privados para la lógica interna
    void cargarAnfitriones();
    void cargarHuespedes();
    void cargarAlojamientos(); // Debe hacerse después de anfitriones
    void cargarReservas();     // Debe hacerse después de huéspedes y alojamientos
    void cargarHistoricoReservas();

    void guardarAnfitriones();
    void guardarHuespedes();
    void guardarAlojamientos();
    void guardarReservas(); // Guardar reservas vigentes
    void guardarHistoricoReservas();

    Anfitrion* buscarAnfitrion(const char* doc);
    Huesped* buscarHuesped(const char* doc);
    Alojamiento* buscarAlojamiento(const char* codigo) ;
    Reserva* buscarReservaVigente(const char* codigoReserva) const; // Necesitaría una lista de reservas vigentes

    void limpiarListaAnfitriones();
    void limpiarListaHuespedes();
    void limpiarListaAlojamientos();
    void limpiarHistoricoReservas();
    void limpiarListaReservasActivas();


    // Helper para generar códigos únicos de reserva
    long long proximoIdReserva; // Simple contador para generar códigos
    char* generarCodigoUnicoReservaInterno();


    // Para los menús y la interacción
    void mostrarMenuAnfitrion(Anfitrion* anfitrion);
    void mostrarMenuHuesped(Huesped* huesped);
    void gestionarIngresoPlataforma();

    // Helpers para funcionalidades específicas
    void realizarProcesoReserva(Huesped* huesped);
    void realizarProcesoAnulacion(void* usuario, bool esAnfitrion);
    void realizarConsultaReservasAnfitrion(Anfitrion* anfitrion);
    void realizarActualizacionHistorico(Anfitrion* anfitrion);


public:
    SistemaUdeAStay();
    ~SistemaUdeAStay();

    // SistemaUdeAStay es un gestor central, no debería ser copiable.
    SistemaUdeAStay(const SistemaUdeAStay&) = delete;
    SistemaUdeAStay& operator=(const SistemaUdeAStay&) = delete;

    void iniciarSistema(); // Carga datos y muestra menú principal

    // Las funcionalidades públicas podrían ser llamadas desde los menús
    // La autenticación es un buen candidato a ser público
    void* autenticarUsuario(const char* documento, const char* password, const char* rol);

    void cargarDatos();    // Pública para ser llamada por iniciarSistema, o privada si prefiere
    void guardarDatos();   // Pública o privada. La haré no-const para permitir medición interna.
    void mostrarMenuPrincipal(); // Pública o privada.


    // Medición de recursos (simplificada)
    void iniciarMedicionIteraciones();
    void incrementarIteraciones(long long cantidad = 1);
    void finalizarMedicionRecursosYMostrar(const char* nombreFuncionalidad);
    void estimarMemoriaActual(); // Suma de sizeof de objetos dinámicos conocidos
};

#endif // SISTEMAUDEASTAY_H
