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

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Codigo para simular que un vehículo REPETIDO salga del parqueadero
    std::vector<std::string> poolPlacasEstacionadas;

    std::cout << "========================================================\n";
    std::cout << " GENERADOR DE PLACAS \n";
    std::cout << "========================================================\n";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 1;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr);

    std::cout << "[INFO] Conectando a " << SERVER_IP << ":" << SERVER_PORT << "...\n";
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "[ERROR] No se pudo conectar al Servidor. Codigo: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "[EXITO] Conectado. Transmitiendo vehiculos cada 2 a 5 segundos...\n\n";
