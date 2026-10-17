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

    while (true) {
        std::string placa;
        bool esReintentoSalida = false;

        // Condicional de 40% de probabilidad de repetir una placa existente para forzar SALIDA
        if (!poolPlacasEstacionadas.empty() && (rand() % 100 < 40)) {
            int idx = rand() % poolPlacasEstacionadas.size();
            placa = poolPlacasEstacionadas[idx];
            poolPlacasEstacionadas.erase(poolPlacasEstacionadas.begin() + idx);
            esReintentoSalida = true;
        } else {
            placa = generarMatriculaAleatoria();
            poolPlacasEstacionadas.push_back(placa);
        }

        std::string marca = MARCAS[rand() % MARCAS.size()];
        std::string color = COLORES[rand() % COLORES.size()];
        std::string hora = obtenerHoraActual();
       // Informacion enviada por el socket: MATRICULA|MARCA|COLOR|HORA
        std::string trama = placa + "|" + marca + "|" + color + "|" + hora;

        std::cout << ">>> Placa: " << placa << " | " << marca << " (" << color << ") | Hora: " << hora;
        std::cout << (esReintentoSalida ? " [PLACA REPETIDA -> SALIDA]\n" : " [NUEVA PLACA -> INGRESO]\n");

        send(sock, trama.c_str(), (int)trama.length(), 0);

        memset(buffer, 0, BUFFER_SIZE);
        int readBytes = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (readBytes > 0) {
            buffer[readBytes] = '\0';
            std::cout << "    <<< [RESPUESTA DEL SERVIDOR]: " << buffer << "\n";
        }

        // Intervalo aleatorio entre 2 y 5 segundos para generar una matricula
        int segundosEspera = 2 + (rand() % 4); // Genera 2, 3, 4 o 5
        Sleep(segundosEspera * 1000);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
