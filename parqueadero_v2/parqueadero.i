%module parqueadero

%{
#include "parqueadero.h"
%}

// Soporte de std::string nativo en Python
%include "std_string.i"

struct EventoInfo {
    int tipo; // 1: INGRESO, 2: LIBERACION, 3: LLENO
    std::string placa;
    std::string marca;
    std::string color;
    std::string celda;
    std::string hora;
    std::string mensaje;
};

int IniciarServidor(int puerto);
bool HayEvento();
EventoInfo ObtenerEvento();
void DetenerServidor();
