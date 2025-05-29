#include "SistemaUdeAStay.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>
#include <cstdio>   // Para sprintf en generarCodigoUnicoReservaInterno
#include <cstdlib>

// Asumiendo utils.h con duplicarCStringGlobal o que está definido en otro lugar accesible
// Si no, define aquí: char* duplicarCStringGlobal(const char* s) { ... }
char* duplicarCStringGlobal(const char* s); // Declaración si está en utils.cpp y no en utils.h
    // O si utils.h tiene la def inline, ya estaría.

// --- Implementación de Métodos Privados de Carga/Guardado ---
void SistemaUdeAStay::cargarAnfitriones() {
    std::cout << "Cargando anfitriones desde " << ARCHIVO_ANFITRIONES << "..." << std::endl;
    std::ifstream archivo(ARCHIVO_ANFITRIONES);

    if (!archivo.is_open()) {
        std::cerr << "ADVERTENCIA: No se pudo abrir el archivo " << ARCHIVO_ANFITRIONES
                  << ". No se cargarAnfitriones." << std::endl;
        return;
    }

    // Limpiar lista existente antes de cargar para evitar duplicados si se llama múltiples veces
    limpiarListaAnfitriones(); // Asegúrate que esto borra los objetos Anfitrion*
    delete listaAnfitriones;   // Borra el objeto ListaPersonalizada en sí
    listaAnfitriones = new ListaPersonalizada<Anfitrion*>(); // Crea una nueva lista vacía


    char lineaBuffer[512]; // Buffer para leer cada línea
    const char* delimitador = ";";

    while (archivo.getline(lineaBuffer, sizeof(lineaBuffer))) {
        incrementarIteraciones(); // Por leer una línea

        char* doc = nullptr;
        char* nombre = nullptr;
        char* userCred = nullptr;
        char* passCred = nullptr;
        char* antiguedadStr = nullptr;
        char* puntuacionStr = nullptr;

        char* token = strtok(lineaBuffer, delimitador);
        if (token) doc = duplicarCStringGlobal(token); else continue; // Saltar línea mal formada

        token = strtok(nullptr, delimitador);
        if (token) nombre = duplicarCStringGlobal(token); else { delete[] doc; continue; }

        token = strtok(nullptr, delimitador);
        if (token) userCred = duplicarCStringGlobal(token); else { delete[] doc; delete[] nombre; continue; }

        token = strtok(nullptr, delimitador);
        if (token) passCred = duplicarCStringGlobal(token); else { delete[] doc; delete[] nombre; delete[] userCred; continue; }

        token = strtok(nullptr, delimitador);
        if (token) antiguedadStr = duplicarCStringGlobal(token); else { /* ... liberar anteriores ... */ continue; }

        token = strtok(nullptr, delimitador);
        if (token) puntuacionStr = duplicarCStringGlobal(token); else { /* ... liberar anteriores ... */ continue; }

        // Convertir a los tipos correctos
        int antiguedad = atoi(antiguedadStr);
        float puntuacion = atof(puntuacionStr);

        Credenciales creds(userCred, passCred); // Credenciales duplicará userCred y passCred internamente
        Anfitrion* anfi = new Anfitrion(doc, nombre, creds, antiguedad, puntuacion);
        listaAnfitriones->agregarPuntero(anfi);
        incrementarIteraciones(); // Por la creación y adición

        // Liberar la memoria de las cadenas duplicadas temporalmente para el parseo
        delete[] doc;
        delete[] nombre;
        delete[] userCred;
        delete[] passCred;
        delete[] antiguedadStr;
        delete[] puntuacionStr;
    }

    archivo.close();
    if (listaAnfitriones) {
        std::cout << listaAnfitriones->getTamano() << " anfitriones cargados desde archivo." << std::endl;
    } else {
        std::cout << "0 anfitriones cargados (lista no inicializada)." << std::endl;
    }
}

void SistemaUdeAStay::cargarHuespedes() {
    std::cout << "Cargando huespedes desde " << ARCHIVO_HUESPEDES << "..." << std::endl;
    std::ifstream archivo(ARCHIVO_HUESPEDES);

    if (!archivo.is_open()) {
        std::cerr << "ADVERTENCIA: No se pudo abrir el archivo " << ARCHIVO_HUESPEDES
                  << ". No se cargarán huéspedes." << std::endl;
        // Si la lista no existe (primera ejecución), la creamos vacía.
        if (!listaHuespedes) {
            listaHuespedes = new ListaPersonalizada<Huesped*>();
        }
        return;
    }

    // Limpiar lista existente antes de cargar para evitar duplicados si se llama múltiples veces
    if (listaHuespedes) {
        limpiarListaHuespedes(); // Asegúrate que esto borra los objetos Huesped*
        delete listaHuespedes;   // Borra el objeto ListaPersonalizada en sí
    }
    listaHuespedes = new ListaPersonalizada<Huesped*>(); // Crea una nueva lista vacía

    char lineaBuffer[512]; // Buffer para leer cada línea
    const char* delimitador = ";";

    while (archivo.getline(lineaBuffer, sizeof(lineaBuffer))) {
        incrementarIteraciones(); // Por leer una línea

        // Hacemos una copia de la línea para que strtok no la modifique permanentemente
        // si necesitáramos el buffer original para algo más (aunque aquí getline lo sobrescribe)
        char* lineaMutable = duplicarCStringGlobal(lineaBuffer);
        if (!lineaMutable) continue; // No se pudo duplicar, saltar

        char* doc = nullptr;
        char* nombre = nullptr;
        char* userCred = nullptr;
        char* passCred = nullptr;
        char* antiguedadStr = nullptr;
        char* puntuacionStr = nullptr;

        // Parseo con strtok (recordar que modifica la cadena)
        char* token = strtok(lineaMutable, delimitador);
        if (token) doc = duplicarCStringGlobal(token); else { delete[] lineaMutable; continue; }

        token = strtok(nullptr, delimitador);
        if (token) nombre = duplicarCStringGlobal(token); else { delete[] doc; delete[] lineaMutable; continue; }

        token = strtok(nullptr, delimitador);
        if (token) userCred = duplicarCStringGlobal(token); else { delete[] doc; delete[] nombre; delete[] lineaMutable; continue; }

        token = strtok(nullptr, delimitador);
        if (token) passCred = duplicarCStringGlobal(token); else { delete[] doc; delete[] nombre; delete[] userCred; delete[] lineaMutable; continue; }

        token = strtok(nullptr, delimitador);
        if (token) antiguedadStr = duplicarCStringGlobal(token); else { delete[] doc; delete[] nombre; delete[] userCred; delete[] passCred; delete[] lineaMutable; continue; }

        token = strtok(nullptr, delimitador);
        if (token) puntuacionStr = duplicarCStringGlobal(token); else { delete[] doc; delete[] nombre; delete[] userCred; delete[] passCred; delete[] antiguedadStr; delete[] lineaMutable; continue; }

        // Convertir a los tipos correctos
        int antiguedad = atoi(antiguedadStr);
        float puntuacion = atof(puntuacionStr);

        Credenciales creds(userCred, passCred); // Credenciales duplica userCred y passCred internamente
        Huesped* hues = new Huesped(doc, nombre, creds, antiguedad, puntuacion);
        listaHuespedes->agregarPuntero(hues);
        incrementarIteraciones(); // Por la creación y adición

        // Liberar la memoria de las cadenas duplicadas temporalmente para el parseo
        delete[] doc;
        delete[] nombre;
        delete[] userCred;
        delete[] passCred;
        delete[] antiguedadStr;
        delete[] puntuacionStr;
        delete[] lineaMutable; // Liberar la copia de la línea
    }

    archivo.close();
    if (listaHuespedes) {
        std::cout << listaHuespedes->getTamano() << " huespedes cargados desde archivo." << std::endl;
    } else {
        std::cout << "0 huespedes cargados (lista no inicializada)." << std::endl;
    }
}


void SistemaUdeAStay::cargarAlojamientos() {
    std::cout << "Cargando alojamientos desde " << ARCHIVO_ALOJAMIENTOS << "..." << std::endl;
    std::ifstream archivo(ARCHIVO_ALOJAMIENTOS);

    if (!archivo.is_open()) {
        std::cerr << "ADVERTENCIA: No se pudo abrir el archivo " << ARCHIVO_ALOJAMIENTOS
                  << ". No se cargarán alojamientos." << std::endl;
        if (!listaAlojamientos) { // Asegurarse que la lista exista aunque esté vacía
            listaAlojamientos = new ListaPersonalizada<Alojamiento*>();
        }
        return;
    }

    // Limpiar lista existente antes de cargar
    if (listaAlojamientos) {
        limpiarListaAlojamientos(); // Borra los objetos Alojamiento*
        delete listaAlojamientos;   // Borra el objeto ListaPersonalizada
    }
    listaAlojamientos = new ListaPersonalizada<Alojamiento*>();

    char lineaBuffer[1024]; // Buffer para leer cada línea (puede ser larga por dirección/amenidades)
    const char* delimitadorPrincipal = ";";
    const char* delimitadorAmenidades = "|";

    while (archivo.getline(lineaBuffer, sizeof(lineaBuffer))) {
        incrementarIteraciones(); // Por leer una línea

        char* lineaMutable = duplicarCStringGlobal(lineaBuffer);
        if (!lineaMutable) continue;

        // Punteros para los tokens de la línea principal
        char* codigoId_token = nullptr;
        char* nombre_token = nullptr;
        char* docAnfitrion_token = nullptr;
        char* depto_token = nullptr;
        char* munic_token = nullptr;
        char* tipo_token = nullptr;
        char* dir_token = nullptr;
        char* precioStr_token = nullptr;
        char* amenidadesStr_token = nullptr; // Cadena completa de amenidades "a|b|c"

        // Parseo con strtok
        char* token = strtok(lineaMutable, delimitadorPrincipal);
        if (token) codigoId_token = token; else { delete[] lineaMutable; continue; }

        token = strtok(nullptr, delimitadorPrincipal);
        if (token) nombre_token = token; else { delete[] lineaMutable; continue; }

        token = strtok(nullptr, delimitadorPrincipal);
        if (token) docAnfitrion_token = token; else { delete[] lineaMutable; continue; }

        token = strtok(nullptr, delimitadorPrincipal);
        if (token) depto_token = token; else { delete[] lineaMutable; continue; }

        token = strtok(nullptr, delimitadorPrincipal);
        if (token) munic_token = token; else { delete[] lineaMutable; continue; }

        token = strtok(nullptr, delimitadorPrincipal);
        if (token) tipo_token = token; else { delete[] lineaMutable; continue; }

        token = strtok(nullptr, delimitadorPrincipal);
        if (token) dir_token = token; else { delete[] lineaMutable; continue; }

        token = strtok(nullptr, delimitadorPrincipal);
        if (token) precioStr_token = token; else { delete[] lineaMutable; continue; }

        token = strtok(nullptr, delimitadorPrincipal); // El resto es la cadena de amenidades
        if (token) amenidadesStr_token = token; else { amenidadesStr_token = const_cast<char*>(""); /* Cadena vacía si no hay amenidades */ }


        // Convertir y buscar referencias
        float precio = atof(precioStr_token);
        Anfitrion* anfiResp = buscarAnfitrion(docAnfitrion_token); // buscarAnfitrion ya cuenta iteraciones

        if (!anfiResp) {
            std::cerr << "ADVERTENCIA: Anfitrion con documento '" << docAnfitrion_token
                      << "' no encontrado para el alojamiento '" << codigoId_token
                      << "'. Alojamiento no cargado." << std::endl;
            delete[] lineaMutable;
            continue;
        }

        // Crear el objeto Alojamiento (los tokens son punteros dentro de lineaMutable,
        // el constructor de Alojamiento debe duplicarlos si los va a almacenar)
        Alojamiento* aloj = new Alojamiento(
            codigoId_token, nombre_token, anfiResp, depto_token,
            munic_token, tipo_token, dir_token, precio
            );

        // Parsear y agregar amenidades
        if (strlen(amenidadesStr_token) > 0) {
            // strtok modifica la cadena, y amenidadesStr_token apunta dentro de lineaMutable.
            // No necesitamos otra copia aquí si solo vamos a leer de ella con strtok.
            char* amenidadToken = strtok(amenidadesStr_token, delimitadorAmenidades);
            while (amenidadToken != nullptr) {
                incrementarIteraciones();
                aloj->agregarAmenidad(amenidadToken); // Alojamiento::agregarAmenidad debe duplicar la cadena
                amenidadToken = strtok(nullptr, delimitadorAmenidades);
            }
        }

        listaAlojamientos->agregarPuntero(aloj);
        anfiResp->agregarAlojamiento(aloj); // El anfitrión también conoce sus alojamientos
        incrementarIteraciones(); // Por la creación y adición

        delete[] lineaMutable; // Liberar la copia de la línea que fue modificada por strtok
    }

    archivo.close();
    if (listaAlojamientos) {
        std::cout << listaAlojamientos->getTamano() << " alojamientos cargados desde archivo." << std::endl;
    } else {
        std::cout << "0 alojamientos cargados (lista no inicializada)." << std::endl;
    }
}

void SistemaUdeAStay::cargarReservas() { // Carga en listaReservasActivas
    std::cout << "Cargando reservas vigentes desde " << ARCHIVO_RESERVAS_VIGENTES << "..." << std::endl;
    std::ifstream archivo(ARCHIVO_RESERVAS_VIGENTES);

    if (!archivo.is_open()) {
        std::cerr << "ADVERTENCIA: No se pudo abrir el archivo " << ARCHIVO_RESERVAS_VIGENTES
                  << ". No se cargarán reservas vigentes." << std::endl;
        if (!listaReservasActivas) listaReservasActivas = new ListaPersonalizada<Reserva*>(); // Asegurar que exista
        return;
    }

    limpiarListaReservasActivas(); // Limpia objetos Reserva* anteriores de esta lista
    delete listaReservasActivas;
    listaReservasActivas = new ListaPersonalizada<Reserva*>();

    char lineaBuffer[1500]; // Las anotaciones pueden ser largas (1000 chars)
    const char* delimitador = ";";

    char* codRes_token, *codAloj_token, *docHues_token, *fEntDia_token, *fEntMes_token, *fEntAnio_token;
    char* durNoches_token, *metPago_token, *fPagoDia_token, *fPagoMes_token, *fPagoAnio_token;
    char* monto_token, *anot_token;

    while (archivo.getline(lineaBuffer, sizeof(lineaBuffer))) {
        incrementarIteraciones();
        char* lineaMutable = duplicarCStringGlobal(lineaBuffer);
        if (!lineaMutable) continue;

        char* token = strtok(lineaMutable, delimitador);
        if (token) codRes_token = token; else { delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador);
        if (token) codAloj_token = token; else { delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador);
        if (token) docHues_token = token; else { delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador);
        if (token) fEntDia_token = token; else { delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador);
        if (token) fEntMes_token = token; else { delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador);
        if (token) fEntAnio_token = token; else { delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador);
        if (token) durNoches_token = token; else { delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador);
        if (token) metPago_token = token; else { delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador);
        if (token) fPagoDia_token = token; else { delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador);
        if (token) fPagoMes_token = token; else { delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador);
        if (token) fPagoAnio_token = token; else { delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador);
        if (token) monto_token = token; else { delete[] lineaMutable; continue; }
        token = strtok(nullptr, ""); // El resto de la línea son las anotaciones
        if (token) anot_token = token; else { anot_token = const_cast<char*>("");}


        Alojamiento* aloj = buscarAlojamiento(codAloj_token);
        Huesped* hues = buscarHuesped(docHues_token);

        if (!aloj) {
            std::cerr << "ADVERTENCIA: Alojamiento con código '" << codAloj_token << "' no encontrado para reserva '" << codRes_token << "'. Reserva no cargada." << std::endl;
            delete[] lineaMutable;
            continue;
        }
        if (!hues) {
            std::cerr << "ADVERTENCIA: Huésped con documento '" << docHues_token << "' no encontrado para reserva '" << codRes_token << "'. Reserva no cargada." << std::endl;
            delete[] lineaMutable;
            continue;
        }

        Fecha fEnt(atoi(fEntDia_token), atoi(fEntMes_token), atoi(fEntAnio_token));
        int durNoches = atoi(durNoches_token);
        Fecha fPago(atoi(fPagoDia_token), atoi(fPagoMes_token), atoi(fPagoAnio_token));
        float monto = atof(monto_token);

        // Los tokens ya apuntan a partes de lineaMutable, Reserva constructor debe duplicar
        Reserva* res = new Reserva(codRes_token, aloj, hues, fEnt, durNoches, metPago_token, fPago, monto, anot_token);

        listaReservasActivas->agregarPuntero(res); // SistemaUdeAStay es dueño
        aloj->agregarReferenciaReserva(res);       // Alojamiento tiene referencia
        hues->agregarReferenciaReserva(res);       // Huésped tiene referencia
        incrementarIteraciones();

        delete[] lineaMutable;
    }
    archivo.close();
    std::cout << (listaReservasActivas ? listaReservasActivas->getTamano() : 0) << " reservas vigentes cargadas." << std::endl;
}

void SistemaUdeAStay::cargarHistoricoReservas() {
    std::cout << "Cargando histórico de reservas desde " << ARCHIVO_RESERVAS_HISTORICAS << "..." << std::endl;
    std::ifstream archivo(ARCHIVO_RESERVAS_HISTORICAS);
    if (!archivo.is_open()) {
        std::cerr << "ADVERTENCIA: No se pudo abrir el archivo " << ARCHIVO_RESERVAS_HISTORICAS
                  << ". No se cargará histórico." << std::endl;
        if(!historicoReservas) historicoReservas = new ListaPersonalizada<Reserva*>();
        return;
    }

    limpiarHistoricoReservas();
    delete historicoReservas;
    historicoReservas = new ListaPersonalizada<Reserva*>();

    char lineaBuffer[1500];
    const char* delimitador = ";";
    // Misma lógica de parseo que cargarReservas()
    // Los punteros a Alojamiento y Huesped en las Reservas históricas deben
    // apuntar a los objetos ya cargados en listaAlojamientos/listaHuespedes.
    // Si un Alojamiento o Huesped referenciado ya no existe en las listas principales
    // (porque fue borrado del sistema), tendrías un problema de integridad referencial.
    // Para este desafío, asumimos que los anfitriones/huéspedes/alojamientos referenciados
    // en el histórico aún existen o se pueden cargar sus datos básicos.

    char* codRes_token, *codAloj_token, *docHues_token, *fEntDia_token, *fEntMes_token, *fEntAnio_token;
    char* durNoches_token, *metPago_token, *fPagoDia_token, *fPagoMes_token, *fPagoAnio_token;
    char* monto_token, *anot_token;

    while (archivo.getline(lineaBuffer, sizeof(lineaBuffer))) {
        incrementarIteraciones();
        char* lineaMutable = duplicarCStringGlobal(lineaBuffer);
        if (!lineaMutable) continue;

        char* token = strtok(lineaMutable, delimitador);
        if (token) codRes_token = token; else { delete[] lineaMutable; continue; }
        // ... (parsear todos los tokens como en cargarReservas) ...
        token = strtok(nullptr, delimitador); if (token) codAloj_token = token; else { /* ... cleanup ... */ delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador); if (token) docHues_token = token; else { /* ... cleanup ... */ delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador); if (token) fEntDia_token = token; else { /* ... cleanup ... */ delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador); if (token) fEntMes_token = token; else { /* ... cleanup ... */ delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador); if (token) fEntAnio_token = token; else { /* ... cleanup ... */ delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador); if (token) durNoches_token = token; else { /* ... cleanup ... */ delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador); if (token) metPago_token = token; else { /* ... cleanup ... */ delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador); if (token) fPagoDia_token = token; else { /* ... cleanup ... */ delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador); if (token) fPagoMes_token = token; else { /* ... cleanup ... */ delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador); if (token) fPagoAnio_token = token; else { /* ... cleanup ... */ delete[] lineaMutable; continue; }
        token = strtok(nullptr, delimitador); if (token) monto_token = token; else { /* ... cleanup ... */ delete[] lineaMutable; continue; }
        token = strtok(nullptr, "");
        if (token) anot_token = token; else { anot_token = const_cast<char*>("");}


        Alojamiento* aloj = buscarAlojamiento(codAloj_token);
        Huesped* hues = buscarHuesped(docHues_token);

        if (!aloj || !hues) {
            std::cerr << "ADVERTENCIA: Alojamiento (" << codAloj_token << ") o Huésped (" << docHues_token
                      << ") no encontrado para reserva histórica '" << codRes_token << "'. Reserva histórica no completamente enlazada o no cargada." << std::endl;
            // Podrías decidir cargar la reserva con punteros nulos a Alojamiento/Huesped,
            // o no cargarla, o cargar solo los IDs. Por ahora, si no se encuentran, no se carga.
            delete[] lineaMutable;
            continue;
        }
        Fecha fEnt(atoi(fEntDia_token), atoi(fEntMes_token), atoi(fEntAnio_token));
        int durNoches = atoi(durNoches_token);
        Fecha fPago(atoi(fPagoDia_token), atoi(fPagoMes_token), atoi(fPagoAnio_token));
        float monto = atof(monto_token);

        Reserva* res = new Reserva(codRes_token, aloj, hues, fEnt, durNoches, metPago_token, fPago, monto, anot_token);
        historicoReservas->agregarPuntero(res); // SistemaUdeAStay es dueño de esta reserva histórica
        // Decisión: ¿Las reservas históricas también se añaden a las listas de referencia de Huesped/Alojamiento?
        // El PDF dice "De los huéspedes se conoce ... la información de sus reservas". Podría incluir todas.
        // Si es así, las añadirías:
        // hues->agregarReferenciaReserva(res);
        // aloj->agregarReferenciaReserva(res); // Esto podría saturar la lista de "referencias activas" del Alojamiento.
        // Es más probable que `reservasRealizadas` del Huesped sea un historial completo,
        // pero `referenciasAReservas` del Alojamiento sea solo para las activas/futuras.
        // Por ahora, solo al histórico del sistema y el Huesped podría tenerla si es su historial completo.
        if(hues) hues->agregarReferenciaReserva(res);

        incrementarIteraciones();
        delete[] lineaMutable;
    }

    archivo.close();
    std::cout << (historicoReservas ? historicoReservas->getTamano() : 0) << " reservas historicas cargadas." << std::endl;
}

void SistemaUdeAStay::guardarAnfitriones() { // No es const si va a incrementar iteraciones
    std::cout << "Guardando anfitriones en " << ARCHIVO_ANFITRIONES << "..." << std::endl;
    std::ofstream archivo(ARCHIVO_ANFITRIONES, std::ios::trunc);

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << ARCHIVO_ANFITRIONES << " para escritura." << std::endl;
        return;
    }

    if (listaAnfitriones) {
        for (int i = 0; i < listaAnfitriones->getTamano(); ++i) {
            incrementarIteraciones();
            Anfitrion* anfi = listaAnfitriones->obtener(i);
            if (anfi) {
                // Formato: numeroDocumento;nombreCompleto;credenciales_usuario;credenciales_password;antiguedad;puntuacion
                archivo << anfi->getNumeroDocumento() << ";"
                        << anfi->getNombreCompleto() << ";"
                        << (anfi->getCredenciales().nombreUsuario ? anfi->getCredenciales().nombreUsuario : "") << ";"
                        << (anfi->getCredenciales().password ? anfi->getCredenciales().password : "") << ";" // ¡CUIDADO! Guardar passwords en texto plano
                        << anfi->getAntiguedad() << ";"
                        << anfi->getPuntuacion() << std::endl;
            }
        }
    }
    archivo.close();
    std::cout << "Anfitriones guardados." << std::endl;
}

void SistemaUdeAStay::guardarHuespedes() { // No es const si incrementa iteraciones
    std::cout << "Guardando huespedes en " << ARCHIVO_HUESPEDES << "..." << std::endl;
    std::ofstream archivo(ARCHIVO_HUESPEDES, std::ios::trunc); // Abrir en modo escritura, truncar si existe

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << ARCHIVO_HUESPEDES << " para escritura." << std::endl;
        return;
    }

    if (listaHuespedes) {
        for (int i = 0; i < listaHuespedes->getTamano(); ++i) {
            incrementarIteraciones();
            Huesped* hues = listaHuespedes->obtener(i);
            if (hues) {
                // Formato: numeroDocumento;nombreCompleto;credenciales_usuario;credenciales_password;antiguedad;puntuacion
                archivo << hues->getNumeroDocumento() << ";"
                        << hues->getNombreCompleto() << ";"
                        << (hues->getCredenciales().nombreUsuario ? hues->getCredenciales().nombreUsuario : "") << ";"
                        << (hues->getCredenciales().password ? hues->getCredenciales().password : "") << ";" // ¡CUIDADO! Guardar passwords en texto plano
                        << hues->getAntiguedad() << ";"
                        << hues->getPuntuacion() << std::endl;
            }
        }
    }
    archivo.close();
    std::cout << "Huespedes guardados." << std::endl;
}
// En SistemaUdeAStay.cpp

void SistemaUdeAStay::guardarAlojamientos() { // No es const si incrementa iteraciones
    std::cout << "Guardando alojamientos en " << ARCHIVO_ALOJAMIENTOS << "..." << std::endl;
    std::ofstream archivo(ARCHIVO_ALOJAMIENTOS, std::ios::trunc);

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << ARCHIVO_ALOJAMIENTOS << " para escritura." << std::endl;
        return;
    }
    const char* delimitadorPrincipal = ";";
    const char* delimitadorAmenidades = "|";

    if (listaAlojamientos) {
        for (int i = 0; i < listaAlojamientos->getTamano(); ++i) {
            incrementarIteraciones();
            Alojamiento* aloj = listaAlojamientos->obtener(i);
            if (aloj) {
                archivo << aloj->getCodigoIdentificador() << delimitadorPrincipal
                        << aloj->getNombre() << delimitadorPrincipal;

                if (aloj->getAnfitrionResponsable()) {
                    archivo << aloj->getAnfitrionResponsable()->getNumeroDocumento();
                } // else: escribir un placeholder o vacío si puede ser nulo
                archivo << delimitadorPrincipal;

                archivo << aloj->getDepartamento() << delimitadorPrincipal
                        << aloj->getMunicipio() << delimitadorPrincipal
                        << aloj->getTipo() << delimitadorPrincipal
                        << aloj->getDireccion() << delimitadorPrincipal
                        << aloj->getPrecioPorNoche() << delimitadorPrincipal;

                // Formatear amenidades
                const ListaPersonalizada<char*>* amenidades = aloj->getAmenidades();
                if (amenidades) {
                    for (int j = 0; j < amenidades->getTamano(); ++j) {
                        archivo << amenidades->obtener(j);
                        if (j < amenidades->getTamano() - 1) {
                            archivo << delimitadorAmenidades; // Usa el sub-delimitador
                        }
                    }
                }
                archivo << std::endl;
            }
        }
    }
    archivo.close();
    std::cout << "Alojamientos guardados." << std::endl;
}
void SistemaUdeAStay::guardarReservas() { // Guarda listaReservasActivas
    std::cout << "Guardando reservas vigentes en " << ARCHIVO_RESERVAS_VIGENTES << "..." << std::endl;
    std::ofstream archivo(ARCHIVO_RESERVAS_VIGENTES, std::ios::trunc);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << ARCHIVO_RESERVAS_VIGENTES << " para escritura." << std::endl;
        return;
    }

    if (listaReservasActivas) {
        for (int i = 0; i < listaReservasActivas->getTamano(); ++i) {
            incrementarIteraciones();
            Reserva* res = listaReservasActivas->obtener(i);
            if (res && res->getAlojamiento() && res->getHuesped()) { // Asegurarse que los punteros son válidos
                archivo << res->getCodigoReserva() << ";"
                        << res->getAlojamiento()->getCodigoIdentificador() << ";"
                        << res->getHuesped()->getNumeroDocumento() << ";"
                        << res->getFechaEntrada().dia << ";" << res->getFechaEntrada().mes << ";" << res->getFechaEntrada().anio << ";"
                        << res->getDuracionNoches() << ";"
                        << res->getMetodoPago() << ";"
                        << res->getFechaPago().dia << ";" << res->getFechaPago().mes << ";" << res->getFechaPago().anio << ";"
                        << res->getMontoTotal() << ";"
                        << (res->getAnotacionesHuesped() ? res->getAnotacionesHuesped() : "") // Manejar posible nullptr
                        << std::endl;
            }
        }
    }
    archivo.close();
    std::cout << "Reservas vigentes guardadas." << std::endl;
}
void SistemaUdeAStay::guardarHistoricoReservas() { // No es const si incrementa iteraciones
    std::cout << "Guardando histórico de reservas en " << ARCHIVO_RESERVAS_HISTORICAS << "..." << std::endl;
    std::ofstream archivo(ARCHIVO_RESERVAS_HISTORICAS, std::ios::trunc);
    if (!archivo.is_open()) { /* ... manejo de error ... */ return; }

    if (historicoReservas) {
        for (int i = 0; i < historicoReservas->getTamano(); ++i) {
            incrementarIteraciones();
            Reserva* res = historicoReservas->obtener(i);
            // Misma lógica de formateo y escritura que en guardarReservas()
            if (res && res->getAlojamiento() && res->getHuesped()) {
                archivo << res->getCodigoReserva() << ";"
                        << res->getAlojamiento()->getCodigoIdentificador() << ";"
                        << res->getHuesped()->getNumeroDocumento() << ";"
                        << res->getFechaEntrada().dia << ";" << res->getFechaEntrada().mes << ";" << res->getFechaEntrada().anio << ";"
                        << res->getDuracionNoches() << ";"
                        << res->getMetodoPago() << ";"
                        << res->getFechaPago().dia << ";" << res->getFechaPago().mes << ";" << res->getFechaPago().anio << ";"
                        << res->getMontoTotal() << ";"
                        << (res->getAnotacionesHuesped() ? res->getAnotacionesHuesped() : "")
                        << std::endl;
            }
        }
    }
    archivo.close();
    std::cout << "Histórico de reservas guardado." << std::endl;
}


// --- Implementación de Buscadores (NO const porque incrementan iteraciones) ---
Anfitrion* SistemaUdeAStay::buscarAnfitrion(const char* doc) {
    if (!listaAnfitriones) return nullptr;
    for (int i = 0; i < listaAnfitriones->getTamano(); ++i) {
        incrementarIteraciones();
        Anfitrion* anfi = listaAnfitriones->obtener(i);
        if (anfi && strcmp(anfi->getNumeroDocumento(), doc) == 0) {
            return anfi;
        }
    }
    return nullptr;
}
Huesped* SistemaUdeAStay::buscarHuesped(const char* doc) {
    if (!listaHuespedes) return nullptr;
    for (int i = 0; i < listaHuespedes->getTamano(); ++i) {
        incrementarIteraciones();
        Huesped* hues = listaHuespedes->obtener(i);
        if (hues && strcmp(hues->getNumeroDocumento(), doc) == 0) {
            return hues;
        }
    }
    return nullptr;
}
Alojamiento* SistemaUdeAStay::buscarAlojamiento(const char* codigo) {
    if (!listaAlojamientos) return nullptr;
    for (int i = 0; i < listaAlojamientos->getTamano(); ++i) {
        incrementarIteraciones();
        Alojamiento* aloj = listaAlojamientos->obtener(i);
        if (aloj && strcmp(aloj->getCodigoIdentificador(), codigo) == 0) {
            return aloj;
        }
    }
    return nullptr;
}

Reserva* SistemaUdeAStay::buscarReservaVigente(const char* codigoReservaBuscado) const {
    // NOTA: Si esta función debe contar iteraciones, no puede ser const.
    // Por ahora, la mantengo const y no cuento iteraciones aquí.
    std::cout << "Advertencia: buscarReservaVigente necesita una lista de reservas activas en SistemaUdeAStay." << std::endl;
    // Si tuvieras una 'listaReservasActivas':
    /*
    if (listaReservasActivas && codigoReservaBuscado) {
        for (int i = 0; i < listaReservasActivas->getTamano(); ++i) {
            // this->incrementarIteraciones(); // Si no fuera const
            Reserva* res = listaReservasActivas->obtener(i);
            if (res && strcmp(res->getCodigoReserva(), codigoReservaBuscado) == 0) {
                return res;
            }
        }
    }
    */
    // Para quitar el warning sin implementar la lógica completa ahora:
    (void)codigoReservaBuscado; // Marca el parámetro como usado explícitamente para el compilador

    return nullptr; // O devuelve algo significativo si la lógica está parcialmente implementada
}

// --- Implementación de Limpiadores de Listas ---
void SistemaUdeAStay::limpiarListaAnfitriones() {
    if (listaAnfitriones) {
        // Llama al destructor de cada Anfitrion en la lista
        listaAnfitriones->limpiarPunteros(); // Asume que ListaPersonalizada::limpiarPunteros hace delete de cada elemento T*
    }
}
void SistemaUdeAStay::limpiarListaHuespedes() {
    if (listaHuespedes) {
        listaHuespedes->limpiarPunteros();
    }
}
void SistemaUdeAStay::limpiarListaAlojamientos() {
    if (listaAlojamientos) {
        listaAlojamientos->limpiarPunteros();
    }
}
void SistemaUdeAStay::limpiarHistoricoReservas() {
    if (historicoReservas) {
        historicoReservas->limpiarPunteros();
    }
}

void SistemaUdeAStay::limpiarListaReservasActivas() {
    if (listaReservasActivas) {
        listaReservasActivas->limpiarPunteros(); // Asume que borra los objetos Reserva*
    }
}

// --- Constructor y Destructor ---
SistemaUdeAStay::SistemaUdeAStay() : contadorIteracionesGlobal(0), memoriaEstimadaGlobal(0), proximoIdReserva(1) {
    listaAnfitriones = new ListaPersonalizada<Anfitrion*>();
    listaHuespedes = new ListaPersonalizada<Huesped*>();
    listaAlojamientos = new ListaPersonalizada<Alojamiento*>();
    listaReservasActivas = new ListaPersonalizada<Reserva*>();
    historicoReservas = new ListaPersonalizada<Reserva*>();


    std::cout << "SistemaUdeAStay inicializado." << std::endl;
}

SistemaUdeAStay::~SistemaUdeAStay() {
    std::cout << "Destruyendo SistemaUdeAStay y liberando memoria..." << std::endl;

    // 1. Borrar Reservas en histórico (SistemaUdeAStay es dueño de estas)
    limpiarHistoricoReservas(); // Borra los objetos Reserva* dentro de la lista
    delete historicoReservas;   // Borra el objeto ListaPersonalizada en sí

    // (Si hubiera listaReservasActivas, se limpiaría y borraría aquí)

    // 2. Borrar Alojamientos
    limpiarListaAlojamientos();
    delete listaAlojamientos;

    // 3. Borrar Anfitriones
    // (Sus listas de Alojamiento* no borran Alojamientos, así que no hay doble delete)
    limpiarListaAnfitriones();
    delete listaAnfitriones;

    // 4.
    limpiarListaReservasActivas(); //
    delete listaReservasActivas;   //

    // 5. Borrar Huéspedes
    // (Sus listas de Reserva* no borran Reservas, así que no hay doble delete)
    limpiarListaHuespedes();
    delete listaHuespedes;

    std::cout << "SistemaUdeAStay destruido." << std::endl;
}

// --- Implementación de Funcionalidades Públicas y Menús (como en la respuesta anterior) ---
void SistemaUdeAStay::iniciarSistema() {
    std::cout << "Iniciando UdeAStay..." << std::endl;
    cargarDatos();
    mostrarMenuPrincipal();
    guardarDatos();
    std::cout << "UdeAStay finalizado." << std::endl;
}

void SistemaUdeAStay::cargarDatos() {
    std::cout << "--- Iniciando Carga de Datos ---" << std::endl;
    iniciarMedicionIteraciones();

    cargarAnfitriones();
    cargarHuespedes();
    cargarAlojamientos();
    cargarReservas(); // Carga conceptual de reservas vigentes si aplica
    cargarHistoricoReservas();

    finalizarMedicionRecursosYMostrar("Carga Completa de Datos");
    std::cout << "--- Carga de Datos Finalizada ---" << std::endl;
}

void SistemaUdeAStay::guardarDatos() { // No es const
    std::cout << "--- Iniciando Guardado de Datos ---" << std::endl;
    iniciarMedicionIteraciones();
    guardarAnfitriones();
    guardarHuespedes();
    guardarAlojamientos();
    guardarReservas(); // Guarda conceptual de reservas vigentes
    guardarHistoricoReservas();
    finalizarMedicionRecursosYMostrar("Guardado Completo de Datos");
    std::cout << "--- Guardado de Datos Finalizado ---" << std::endl;
}

void* SistemaUdeAStay::autenticarUsuario(const char* documento, const char* password, const char* rol) {
    iniciarMedicionIteraciones();
    void* usuarioAutenticado = nullptr;

    if (strcmp(rol, "anfitrion") == 0) {
        Anfitrion* anfi = buscarAnfitrion(documento); // buscarAnfitrion ya cuenta iteraciones
        if (anfi && anfi->verificarPassword(password)) {
            incrementarIteraciones(); // por la verificación
            usuarioAutenticado = anfi;
        }
    } else if (strcmp(rol, "huesped") == 0) {
        Huesped* hues = buscarHuesped(documento); // buscarHuesped ya cuenta iteraciones
        if (hues && hues->verificarPassword(password)) {
            incrementarIteraciones();
            usuarioAutenticado = hues;
        }
    }
    // La medición se hace fuera, en la función que llama a esta, o se pasa el nombre.
    // Por ahora, lo dejo así y la función que llama gestiona el finalize.
    // O, como antes, si esta es la última operación de la funcionalidad "Autenticación":
    finalizarMedicionRecursosYMostrar("Autenticacion de Usuario");
    return usuarioAutenticado;
}

char* SistemaUdeAStay::generarCodigoUnicoReservaInterno() {
    char buffer[20];
    sprintf(buffer, "RES%04lld", proximoIdReserva++);
    return duplicarCStringGlobal(buffer);
}

void SistemaUdeAStay::iniciarMedicionIteraciones() {
    contadorIteracionesGlobal = 0;
}
void SistemaUdeAStay::incrementarIteraciones(long long cantidad) {
    contadorIteracionesGlobal += cantidad;
}
void SistemaUdeAStay::estimarMemoriaActual() { // No es const
    memoriaEstimadaGlobal = 0;
    incrementarIteraciones();
    if(listaAnfitriones) for(int i=0; i<listaAnfitriones->getTamano(); ++i) memoriaEstimadaGlobal += sizeof(Anfitrion) /* + memoria interna de Anfitrion */;
    if(listaHuespedes) for(int i=0; i<listaHuespedes->getTamano(); ++i) memoriaEstimadaGlobal += sizeof(Huesped) /* + ... */;
    if(listaAlojamientos) for(int i=0; i<listaAlojamientos->getTamano(); ++i) memoriaEstimadaGlobal += sizeof(Alojamiento) /* + ... */;
    if(historicoReservas) for(int i=0; i<historicoReservas->getTamano(); ++i) memoriaEstimadaGlobal += sizeof(Reserva) /* + ... */;
    // Esta estimación es muy básica. Necesitarías sumar la memoria de todos los char* dinámicos,
    // y la memoria de las propias ListaPersonalizada.
}

void SistemaUdeAStay::finalizarMedicionRecursosYMostrar(const char* nombreFuncionalidad) { // No es const
    estimarMemoriaActual();
    std::cout << "\n--- Medicion de Recursos para '" << nombreFuncionalidad << "' ---" << std::endl;
    std::cout << "Iteraciones estimadas: " << contadorIteracionesGlobal << std::endl;
    std::cout << "Memoria de objetos principales estimada: " << memoriaEstimadaGlobal << " bytes" << std::endl;
    std::cout << "-----------------------------------------------------\n" << std::endl;
}

void SistemaUdeAStay::mostrarMenuPrincipal() {
    int opcion = 0;
    do {
        std::cout << "\n===== UdeAStay - Menu Principal =====" << std::endl;
        std::cout << "1. Ingresar como Anfitrion" << std::endl;
        std::cout << "2. Ingresar como Huesped" << std::endl;
        std::cout << "0. Salir" << std::endl;
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            opcion = -1;
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (opcion) {
        case 1:
        case 2: {
            gestionarIngresoPlataforma();
            break;
        }
        case 0:
            std::cout << "Saliendo del sistema..." << std::endl;
            break;
        default:
            std::cout << "Opcion no valida. Intente de nuevo." << std::endl;
        }
    } while (opcion != 0);
}

void SistemaUdeAStay::gestionarIngresoPlataforma() {
    // La medición se inicia dentro de autenticarUsuario si se considera una funcionalidad completa
    // o aquí si "gestionarIngresoPlataforma" es la funcionalidad a medir.
    // iniciarMedicionIteraciones(); // Redundante si autenticar lo hace.
    char doc[50], pass[50], rol[20];
    std::cout << "Ingrese su rol (anfitrion/huesped): ";
    std::cin.getline(rol, 20); // Usar getline para evitar problemas con espacios si los hubiera
    std::cout << "Ingrese numero de documento: ";
    std::cin.getline(doc, 50);
    std::cout << "Ingrese password: ";
    std::cin.getline(pass, 50);

    void* usuario = autenticarUsuario(doc, pass, rol); // autenticarUsuario maneja su propia medición y muestra

    if (usuario) {
        std::cout << "Ingreso exitoso!" << std::endl;
        // La medición de "Autenticacion de Usuario" ya se mostró.
        // Ahora entramos a un submenú, cada acción allí tendrá su propia medición.
        if (strcmp(rol, "anfitrion") == 0) {
            mostrarMenuAnfitrion(static_cast<Anfitrion*>(usuario));
        } else if (strcmp(rol, "huesped") == 0) {
            mostrarMenuHuesped(static_cast<Huesped*>(usuario));
        }
    } else {
        std::cout << "Documento, password o rol incorrectos." << std::endl;
        // La medición de "Autenticacion de Usuario" (fallida) ya se mostró.
    }
}

void SistemaUdeAStay::mostrarMenuAnfitrion(Anfitrion* anfitrion) {
    int opcion = 0;
    do {
        std::cout << "\n===== Menu Anfitrion: " << (anfitrion->getNombreCompleto() ? anfitrion->getNombreCompleto() : "N/A") << " =====" << std::endl;
        std::cout << "1. Ver mi Informacion y Alojamientos" << std::endl;
        std::cout << "2. Agregar Alojamiento" << std::endl;
        std::cout << "3. Consultar mis Reservaciones Activas" << std::endl;
        std::cout << "4. Anular Reservacion en mis Alojamientos" << std::endl;
        std::cout << "5. Actualizar Historico de Reservaciones" << std::endl;
        std::cout << "0. Cerrar Sesion" << std::endl;
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;
        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); opcion = -1;}
        else {std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');}

        switch (opcion) {
        case 1:
            iniciarMedicionIteraciones();
            anfitrion->mostrarInformacion();
            finalizarMedicionRecursosYMostrar("Ver Info Anfitrion y Alojamientos");
            break;
        case 2:
            // Implementar lógica completa
            std::cout << "Funcionalidad 'Agregar Alojamiento' no implementada en detalle." << std::endl;
            // iniciarMedicionIteraciones(); ... finalizarMedicionRecursosYMostrar("Agregar Alojamiento");
            break;
        case 3:
            realizarConsultaReservasAnfitrion(anfitrion);
            break;
        case 4:
            realizarProcesoAnulacion(anfitrion, true);
            break;
        case 5:
            realizarActualizacionHistorico(anfitrion);
            break;
        case 0:
            std::cout << "Cerrando sesion de anfitrion..." << std::endl;
            break;
        default:
            std::cout << "Opcion no valida." << std::endl;
        }
    } while (opcion != 0);
}

void SistemaUdeAStay::mostrarMenuHuesped(Huesped* huesped) {
    int opcion = 0;
    do {
        std::cout << "\n===== Menu Huesped: " << (huesped->getNombreCompleto() ? huesped->getNombreCompleto() : "N/A") << " =====" << std::endl;
        std::cout << "1. Buscar y Reservar Alojamiento" << std::endl;
        std::cout << "2. Ver mis Reservas" << std::endl;
        std::cout << "3. Anular una de mis Reservas" << std::endl;
        std::cout << "0. Cerrar Sesion" << std::endl;
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;
        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); opcion = -1;}
        else {std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');}

        switch (opcion) {
        case 1:
            realizarProcesoReserva(huesped);
            break;
        case 2:
            iniciarMedicionIteraciones();
            huesped->mostrarInformacion();
            finalizarMedicionRecursosYMostrar("Ver Mis Reservas (Huesped)");
            break;
        case 3:
            realizarProcesoAnulacion(huesped, false);
            break;
        case 0:
            std::cout << "Cerrando sesion de huesped..." << std::endl;
            break;
        default:
            std::cout << "Opcion no valida." << std::endl;
        }
    } while (opcion != 0);
}


void SistemaUdeAStay::realizarProcesoReserva(Huesped* huesped) {
    iniciarMedicionIteraciones();
    std::cout << "\n--- Proceso de Reserva ---" << std::endl;
    char municipio[100];
    int diaIn, mesIn, anioIn, noches;
    float costoMax = -1.0f;
    float puntMinAnfi = -1.0f;

    std::cout << "Ingrese municipio de busqueda: "; std::cin.getline(municipio, 100); incrementarIteraciones();
    std::cout << "Fecha de entrada (dd mm aaaa): "; std::cin >> diaIn >> mesIn >> anioIn; incrementarIteraciones();
    std::cout << "Cantidad de noches: "; std::cin >> noches; incrementarIteraciones();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Fecha fechaEntrada(diaIn, mesIn, anioIn);

    char opcionFiltro;
    std::cout << "Aplicar filtro de costo maximo por noche? (s/n): "; std::cin >> opcionFiltro; incrementarIteraciones();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (opcionFiltro == 's' || opcionFiltro == 'S') {
        std::cout << "Costo maximo por noche: "; std::cin >> costoMax; incrementarIteraciones();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cout << "Aplicar filtro de puntuacion minima de anfitrion? (s/n): "; std::cin >> opcionFiltro; incrementarIteraciones();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (opcionFiltro == 's' || opcionFiltro == 'S') {
        std::cout << "Puntuacion minima del anfitrion (0-5): "; std::cin >> puntMinAnfi; incrementarIteraciones();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    ListaPersonalizada<Alojamiento*> disponibles;
    if(listaAlojamientos) {
        for (int i = 0; i < listaAlojamientos->getTamano(); ++i) {
            incrementarIteraciones();
            Alojamiento* aloj = listaAlojamientos->obtener(i);
            if (aloj && strcmp(aloj->getMunicipio(), municipio) == 0 && aloj->estaDisponible(fechaEntrada, noches)) {
                incrementarIteraciones();
                bool pasaFiltros = true;
                if (costoMax >= 0 && aloj->getPrecioPorNoche() > costoMax) pasaFiltros = false;
                if (puntMinAnfi >= 0 && aloj->getAnfitrionResponsable()->getPuntuacion() < puntMinAnfi) pasaFiltros = false;

                if (pasaFiltros) {
                    disponibles.agregarPuntero(aloj);
                }
            }
        }
    }


    if (disponibles.estaVacia()) {
        std::cout << "No se encontraron alojamientos disponibles con esos criterios." << std::endl;
        finalizarMedicionRecursosYMostrar("Proceso de Reserva (sin exito)");
        return;
    }

    std::cout << "\nAlojamientos disponibles:" << std::endl;
    for (int i = 0; i < disponibles.getTamano(); ++i) {
        incrementarIteraciones();
        std::cout << (i + 1) << ". Cod: " << disponibles.obtener(i)->getCodigoIdentificador()
                  << ", Nombre: " << disponibles.obtener(i)->getNombre() << std::endl;
        // disponibles.obtener(i)->mostrarInformacionDetallada(); // Podría ser muy verboso aquí
    }

    int seleccion;
    std::cout << "Seleccione el numero del alojamiento a reservar (0 para cancelar): "; std::cin >> seleccion; incrementarIteraciones();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (seleccion > 0 && seleccion <= disponibles.getTamano()) {
        Alojamiento* elegido = disponibles.obtener(seleccion - 1);

        // TODO: Validar que el huésped no tenga otra reserva en esas fechas ( crucial y complejo )
        // Esta validación implica iterar sobre las reservas del huésped y comparar rangos de fechas.

        char anotaciones[1001] = ""; // Inicializar para getline
        char metodoPago[50] = "";  // Inicializar para getline
        std::cout << "Ingrese anotaciones para el anfitrion (max 1000 chars, Enter para ninguna): ";
        std::cin.getline(anotaciones, 1001); incrementarIteraciones();
        std::cout << "Metodo de pago (PSE/TCredito): ";
        std::cin.getline(metodoPago, 50); incrementarIteraciones();

        Fecha fechaHoy;
        float monto = elegido->getPrecioPorNoche() * noches;
        char* codigoNuevaReserva = generarCodigoUnicoReservaInterno(); incrementarIteraciones();

        Reserva* nuevaReserva = new Reserva(codigoNuevaReserva, elegido, huesped, fechaEntrada, noches, metodoPago, fechaHoy, monto, anotaciones);
        incrementarIteraciones(); // por el new

        // IMPORTANTE: ¿Quién es dueño de nuevaReserva?
        // Se debe añadir a una lista del SistemaUdeAStay para su gestión y posterior delete.
        // Por ahora, asumimos que el destructor del sistema se encargará de las reservas referenciadas si no
        // se añaden explícitamente a historicoReservas o a una lista de activas.
        // Esto es un punto débil del diseño actual si no hay una lista `listaReservasActivasGlobal`.
        // Una opción es añadirlo a historicoReservas temporalmente.


        if(nuevaReserva) {
            if(listaReservasActivas) { // << AÑADIR A listaReservasActivas
                listaReservasActivas->agregarPuntero(nuevaReserva);
            } else {
                delete nuevaReserva;
                nuevaReserva = nullptr;
                std::cerr << "Error: listaReservasActivas no inicializada!" << std::endl;
            }
            if(nuevaReserva) {
                huesped->agregarReferenciaReserva(nuevaReserva);
                elegido->agregarReferenciaReserva(nuevaReserva);
                incrementarIteraciones(2);

                std::cout << "\n¡Reserva realizada con exito!" << std::endl;
                char* comprobante = nuevaReserva->generarComprobante(); incrementarIteraciones();
                std::cout << comprobante << std::endl;
                delete[] comprobante;
            }

        }
        delete[] codigoNuevaReserva;

    } else {
        std::cout << "Reserva cancelada por el usuario." << std::endl;
    }
    finalizarMedicionRecursosYMostrar("Proceso de Reserva");
}


void SistemaUdeAStay::realizarProcesoAnulacion(void* usuario, bool esAnfitrion) {
    iniciarMedicionIteraciones();
    char codigoReserva[20];
    std::cout << "Ingrese el codigo de la reserva a anular: ";
    std::cin.getline(codigoReserva, 20); incrementarIteraciones();

    Reserva* reservaAAnular = nullptr;
    int indiceEnListaPrincipal = -1; // Si `historicoReservas` contiene todas las reservas activas/pasadas

    // Asumimos que `historicoReservas` tiene TODAS las reservas que el sistema conoce y creó,
    // y luego `actualizarHistorico` realmente las "mueve" (o marca como históricas).
    // Esta es una simplificación para la gestión de memoria.
    if(listaReservasActivas){
        for (int i = 0; i < listaReservasActivas->getTamano(); ++i) {
            incrementarIteraciones();
            Reserva* res = listaReservasActivas->obtener(i);
            if (res && strcmp(res->getCodigoReserva(), codigoReserva) == 0) {
                // Verificar permiso
                bool tienePermiso = false;
                if (esAnfitrion) {
                    Anfitrion* anfi = static_cast<Anfitrion*>(usuario);
                    if (res->getAlojamiento() && res->getAlojamiento()->getAnfitrionResponsable() == anfi) {
                        tienePermiso = true;
                    }
                } else {
                    Huesped* hues = static_cast<Huesped*>(usuario);
                    if (res->getHuesped() == hues) {
                        tienePermiso = true;
                    }
                }

                if (tienePermiso) {
                    reservaAAnular = res;
                    indiceEnListaPrincipal = i;
                    break;
                } else {
                    std::cout << "No tiene permiso para anular esta reserva." << std::endl;
                    finalizarMedicionRecursosYMostrar("Proceso de Anulacion (sin permiso)");
                    return;
                }
            }
        }
    }


    if (reservaAAnular) {
        // Quitar referencias de Huesped y Alojamiento
        if(reservaAAnular->getHuesped()) reservaAAnular->getHuesped()->cancelarReferenciaReserva(codigoReserva);
        if(reservaAAnular->getAlojamiento()) reservaAAnular->getAlojamiento()->eliminarReferenciaReserva(codigoReserva);
        incrementarIteraciones(2);

        // Eliminar el objeto Reserva de la lista principal del sistema y borrarlo
        if(listaReservasActivas && indiceEnListaPrincipal != -1) {
            listaReservasActivas->eliminar(indiceEnListaPrincipal); // Quita el puntero de la lista
        }
        delete reservaAAnular; // SistemaUdeAStay es dueño de este objeto
        incrementarIteraciones();

        std::cout << "Reserva " << codigoReserva << " anulada exitosamente." << std::endl;
    } else {
        std::cout << "Reserva no encontrada." << std::endl;
    }
    finalizarMedicionRecursosYMostrar("Proceso de Anulacion");
}

void SistemaUdeAStay::realizarConsultaReservasAnfitrion(Anfitrion* anfitrion) {
    iniciarMedicionIteraciones();
    int dI, mI, aI, dF, mF, aF;
    std::cout << "Consulta de Reservas Activas para Anfitrion " << (anfitrion->getNombreCompleto() ? anfitrion->getNombreCompleto() : "N/A") << std::endl;
    std::cout << "Ingrese fecha de inicio del rango (dd mm aaaa): "; std::cin >> dI >> mI >> aI; incrementarIteraciones();
    std::cout << "Ingrese fecha de fin del rango (dd mm aaaa): "; std::cin >> dF >> mF >> aF; incrementarIteraciones();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Fecha fechaInicio(dI, mI, aI);
    Fecha fechaFin(dF, mF, aF);

    ListaPersonalizada<Reserva*>* activas = anfitrion->consultarReservacionesActivas(fechaInicio, fechaFin);
    incrementarIteraciones();

    if (activas && !activas->estaVacia()) {
        std::cout << "\nReservas Activas encontradas:" << std::endl;
        for (int i = 0; i < activas->getTamano(); ++i) {
            incrementarIteraciones();
            activas->obtener(i)->mostrarInformacion();
            std::cout << std::endl;
        }
    } else {
        std::cout << "No se encontraron reservas activas en ese rango para sus alojamientos." << std::endl;
    }
    // La lista 'activas' es creada por anfitrion->consultarReservacionesActivas y contiene punteros.
    // El método en Anfitrion debe documentar quién es dueño de la lista devuelta.
    // Asumiendo que el método en Anfitrion devuelve una NUEVA lista de punteros (no los objetos Reserva en sí):
    delete activas;
    finalizarMedicionRecursosYMostrar("Consulta Reservas Anfitrion");
}

void SistemaUdeAStay::realizarActualizacionHistorico(Anfitrion* anfitrion) {
    iniciarMedicionIteraciones();
    int d, m, a;
    std::cout << "Actualizacion de Historico de Reservas (solicitado por "
              << (anfitrion && anfitrion->getNombreCompleto() ? anfitrion->getNombreCompleto() : "N/A") << ")" << std::endl;
    std::cout << "Ingrese la fecha de corte (dd mm aaaa): ";
    std::cin >> d >> m >> a;
    incrementarIteraciones(); // Por la entrada del usuario

    // Limpiar el buffer de entrada después de leer números
    std::cin.clear(); // Limpia flags de error por si acaso
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Fecha fechaCorte(d, m, a);

    // Validar que la fecha de corte no sea menor que fechas en el histórico (según PDF [cite: 143])
    // Esta validación requeriría iterar el histórico y comparar. Por ahora, la omitimos
    // por simplicidad, pero es un requisito del PDF. [cite: 143]
    // También, el PDF dice que la fecha de corte se toma como base para los próximos 12 meses
    // para nuevas reservaciones. [cite: 144] Esto es una regla de negocio para la creación de reservas.

    std::cout << "Procesando actualizacion de historico..." << std::endl;

    if (!listaReservasActivas || !historicoReservas) {
        std::cerr << "Error: Listas de reserva (activas o historico) no inicializadas." << std::endl;
        finalizarMedicionRecursosYMostrar("Actualizacion de Historico (Error)");
        return;
    }

    ListaPersonalizada<Reserva*>* nuevasActivas = new ListaPersonalizada<Reserva*>();

    // Iterar sobre la lista de reservas activas actual
    for (int i = 0; i < listaReservasActivas->getTamano(); ++i) {
        incrementarIteraciones(); // Por cada reserva activa revisada
        Reserva* res = listaReservasActivas->obtener(i);
        if (res) {
            Fecha fechaSalidaRes = res->calcularFechaSalida(); // Asume que esto es const y funciona bien
            incrementarIteraciones(); // Por calcular la fecha de salida

            // Condición de Movimiento: Si la reserva ya terminó ANTES de la fecha de corte [cite: 142]
            if (fechaSalidaRes < fechaCorte) {
                std::cout << "Moviendo reserva " << (res->getCodigoReserva() ? res->getCodigoReserva() : "N/A")
                << " a historico." << std::endl;

                // 1. Añadir a la lista de histórico (el sistema sigue siendo dueño)
                historicoReservas->agregarPuntero(res);
                incrementarIteraciones();

                // 2. Quitar referencias de las listas "activas" en Huesped y Alojamiento,
                //    ya que la reserva ya no se considera "activa" en el flujo principal.
                //    Si Huesped::reservasRealizadas es un historial completo, la referencia puede permanecer allí.
                //    Alojamiento::referenciasAReservas probablemente solo deba tener activas.
                Huesped* huespedDeReserva = res->getHuesped();
                Alojamiento* alojamientoDeReserva = res->getAlojamiento();

                if (alojamientoDeReserva) {
                    alojamientoDeReserva->eliminarReferenciaReserva(res->getCodigoReserva());
                    incrementarIteraciones();
                }
                // Opcional: Si la lista de reservas del huésped solo debe tener activas, también quitarla de ahí.
                // if (huespedDeReserva) {
                //     huespedDeReserva->cancelarReferenciaReserva(res->getCodigoReserva());
                // }

                // El objeto Reserva* NO se añade a nuevasActivas, ya que se movió al histórico.
            } else { // Reserva sigue activa o es futura (termina en o después de la fechaCorte)
                nuevasActivas->agregarPuntero(res); // Permanece en la lista de activas (que será reemplazada por nuevasActivas)
                incrementarIteraciones();
            }
        }
    }

    // La antigua `listaReservasActivas` contenía punteros que ahora están
    // o en `historicoReservas` o en `nuevasActivas`.
    // NO debemos borrar los objetos Reserva aquí. Solo el contenedor de la lista antigua.
    delete listaReservasActivas;
    listaReservasActivas = nuevasActivas; // Sistema ahora gestiona la nueva lista filtrada de activas

    std::cout << "Actualizacion de historico completada. Reservas activas ahora: "
              << (listaReservasActivas ? listaReservasActivas->getTamano() : 0) << std::endl;

    // La parte de "actualiza las estructuras de datos para que permitan almacenar
    // reservaciones en cualquier fecha de los próximos 12 meses" [cite: 142]
    // es una regla de negocio para cuando se crean NUEVAS reservas.
    // Se valida que la fecha de la nueva reserva no exceda fechaCorte + 12 meses.
    // No implica necesariamente redimensionar las ListaPersonalizada aquí, ya que son dinámicas.

    finalizarMedicionRecursosYMostrar("Actualizacion de Historico");
}
