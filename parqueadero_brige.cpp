#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

#define TOTAL_CELDAS 20
#define BUFFER_SIZE 1024

struct CeldaInfo {
    int index;
    bool ocupada;
    std::string placa;
    std::string marca;
    std::string color;
    std::string horaIngreso;
};

struct EventoParqueadero {
    int tipo; // 1: INGRESO, 2: LIBERACION, 3: LLENO
    char placa[16];
    char marca[32];
    char color[32];
    char celda[16];
    char hora[16];
    char mensaje[128];
};

static std::vector<CeldaInfo> celdas(TOTAL_CELDAS);
static std::queue<EventoParqueadero> colaEventos;
static std::mutex mutexParqueadero;
static std::thread hiloServidor;
static bool servidorActivo = false;
static SOCKET servidor_socket = INVALID_SOCKET;

void bucleServidorTCP(int puerto) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    servidor_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in direccion;
    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(puerto);

    bind(servidor_socket, (sockaddr*)&direccion, sizeof(direccion));
    listen(servidor_socket, 5);

    while (servidorActivo) {
        sockaddr_in clienteAddr;
        int clienteLen = sizeof(clienteAddr);
        SOCKET clienteSock = accept(servidor_socket, (sockaddr*)&clienteAddr, &clienteLen);
        if (clienteSock == INVALID_SOCKET) continue;

        char buffer[BUFFER_SIZE];
        while (servidorActivo) {
            memset(buffer, 0, BUFFER_SIZE);
            int bytesRecv = recv(clienteSock, buffer, BUFFER_SIZE - 1, 0);
            if (bytesRecv <= 0) break;

            buffer[bytesRecv] = '\0';
            std::stringstream ss(buffer);
            std::string placa, marca, color, hora;
            std::getline(ss, placa, '|');
            std::getline(ss, marca, '|');
            std::getline(ss, color, '|');
            std::getline(ss, hora, '|');

            std::string respuesta = "";

            {
                std::lock_guard<std::mutex> lock(mutexParqueadero);

                // REGLA: ¿LA PLACA YA EXISTE? -> LIBERAR CELDA
                int celdaEncontrada = -1;
                for (int i = 0; i < TOTAL_CELDAS; i++) {
                    if (celdas[i].ocupada && celdas[i].placa == placa) {
                        celdaEncontrada = i;
                        break;
                    }
                }
