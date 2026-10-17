include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "10.2.10.200"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

const std::vector<std::string> MARCAS = {"Toyota", "Chevrolet", "Mazda", "Nissan", "Renault", "Ford", "Kia", "BMW"};
const std::vector<std::string> COLORES = {"Blanco", "Negro", "Rojo", "Azul", "Gris", "Plata"};

// Genera matrículas
std::string generarMatriculaAleatoria() {
    std::string placa = "";
    for (int i = 0; i < 3; i++) placa += (char)('A' + (rand() % 26));
    for (int i = 0; i < 3; i++) placa += std::to_string(rand() % 10);
    return placa;
}

// codigo para la hora 
std::string obtenerHoraActual() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    struct tm parts;
    localtime_s(&parts, &now_c);

    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << parts.tm_hour << ":"
       << std::setfill('0') << std::setw(2) << parts.tm_min << ":"
       << std::setfill('0') << std::setw(2) << parts.tm_sec;
    return ss.str();
}
