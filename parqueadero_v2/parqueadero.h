#ifndef PARQUEADERO_H
#define PARQUEADERO_H

#include <string>

struct EventoInfo {
    int tipo;
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

#endif
