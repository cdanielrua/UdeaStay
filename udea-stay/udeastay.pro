TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Alojamiento.cpp \
        Anfitrion.cpp \
        Credenciales.cpp \
        Fecha.cpp \
        Huesped.cpp \
        Reserva.cpp \
        SistemaUdeAStay.cpp \
        main.cpp \
        utils.cpp

HEADERS += \
    Alojamiento.h \
    Anfitrion.h \
    Credenciales.h \
    Fecha.h \
    Huesped.h \
    ListaPersonalizada.h \
    Reserva.h \
    SistemaUdeAStay.h \
    utils.h
