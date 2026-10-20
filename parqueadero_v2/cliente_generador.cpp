#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1" // Cambiar por la IP del servidor si esta en otra PC
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

// Marcas y colores compatibles 
static const char* MARCAS[] = {"Toyota", "Chevrolet", "Mazda", "Nissan", "Renault", "Ford", "Kia", "BMW"};
static const int NUM_MARCAS = sizeof(MARCAS) / sizeof(MARCAS[0]);

static const char* COLORES[] = {"Blanco", "Negro", "Rojo", "Azul", "Gris", "Plata"};
static const int NUM_COLORES = sizeof(COLORES) / sizeof(COLORES[0]);

// Genera matricula(ABC123)
std::string generarMatriculaAleatoria() {
    std::string placa = "";
    for (int i = 0; i < 3; i++) placa += (char)('A' + (rand() % 26));
    for (int i = 0; i < 3; i++) placa += (char)('0' + (rand() % 10));
    return placa;
}

// Codigo para hora-formato HH:MM:SS
std::string obtenerHoraActual() {
    std::time_t t = std::time(NULL);
    std::tm* now = std::localtime(&t);
    char buf[16];
    if (now != NULL) {
        std::strftime(buf, sizeof(buf), "%H:%M:%S", now);
        return std::string(buf);
    }
    return "12:00:00";
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));

    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Pool de placas generadas para repetir y forzar SALIDAS
    std::vector<std::string> poolPlacasEstacionadas;

    std::cout << "========================================================\n";
    std::cout << "  MODULO C++ GENERADOR DE PLACAS (CLIENTE TCP)\n";
    std::cout << "========================================================\n";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[ERROR] Fallo WSAStartup. Codigo: " << WSAGetLastError() << std::endl;
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "[ERROR] Fallo al crear socket. Codigo: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    // inet_addr es universal en todas las versiones de Windows
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    if (serv_addr.sin_addr.s_addr == INADDR_NONE) {
        std::cerr << "[ERROR] Direccion IP invalida: " << SERVER_IP << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "[INFO] Conectando a " << SERVER_IP << ":" << SERVER_PORT << "...\n";
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "[ERROR] No se pudo conectar al Servidor (¿Esta activo el visualizador?). Codigo: " 
                  << WSAGetLastError() << std::endl;
        std::cerr << "        Recuerda iniciar PRIMERO: python visualizador.py\n" << std::endl;
        closesocket(sock);
        WSACleanup();
        system("pause");
        return 1;
    }

    std::cout << "[EXITO] Conectado. Transmitiendo vehiculos cada 2 a 5 segundos...\n\n";

    while (true) {
        std::string placa;
        bool esReintentoSalida = false;

        // Si ya hay autos dentro, 40% de probabilidad de repetir una placa para forzar salida
        if (!poolPlacasEstacionadas.empty() && (rand() % 100 < 40)) {
            int idx = rand() % poolPlacasEstacionadas.size();
            placa = poolPlacasEstacionadas[idx];
            poolPlacasEstacionadas.erase(poolPlacasEstacionadas.begin() + idx);
            esReintentoSalida = true;
        } else {
            placa = generarMatriculaAleatoria();
            poolPlacasEstacionadas.push_back(placa);
        }

        std::string marca = MARCAS[rand() % NUM_MARCAS];
        std::string color = COLORES[rand() % NUM_COLORES];
        std::string hora = obtenerHoraActual();

        // Trama enviada por el socket: MATRICULA|MARCA|COLOR|HORA
        std::string trama = placa + "|" + marca + "|" + color + "|" + hora;

        std::cout << ">>> Placa: " << placa << " | " << marca << " (" << color << ") | Hora: " << hora;
        std::cout << (esReintentoSalida ? " [PLACA REPETIDA -> SALIDA]\n" : " [NUEVA PLACA -> INGRESO]\n");

        int bytesSent = send(sock, trama.c_str(), (int)trama.length(), 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "[ERROR] Error al enviar trama por el socket. Codigo: " << WSAGetLastError() << std::endl;
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        int readBytes = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (readBytes > 0) {
            buffer[readBytes] = '\0';
            std::cout << "    <<< [RESPUESTA DEL SERVIDOR]: " << buffer << "\n";
        } else {
            std::cerr << "[AVISO] Conexion cerrada por el servidor.\n";
            break;
        }

        // Intervalo aleatorio entre 2 y 5 segundos
        int segundosEspera = 2 + (rand() % 4); // Genera 2, 3, 4 o 5
        Sleep(segundosEspera * 1000);
    }

    closesocket(sock);
    WSACleanup();
    system("pause");
    return 0;
}
