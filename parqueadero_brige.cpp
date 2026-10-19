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

               EventoParqueadero ev;
                memset(&ev, 0, sizeof(EventoParqueadero));
                strcpy_s(ev.placa, placa.c_str());
                strcpy_s(ev.marca, marca.c_str());
                strcpy_s(ev.color, color.c_str());
                strcpy_s(ev.hora, hora.c_str());

                if (celdaEncontrada != -1) {
                    // 1. PLACA REPETIDA: LIBERAR
                    celdas[celdaEncontrada].ocupada = false;
                    std::string nombreCelda = "C-" + std::to_string(celdaEncontrada + 1);
                    strcpy_s(ev.celda, nombreCelda.c_str());
                    ev.tipo = 2; // LIBERACION
                    strcpy_s(ev.mensaje, "Salida registrada. Celda liberada.");
                    respuesta = "LIBERADO|" + nombreCelda + "|SALIDA_EXITOSA\n";
                    colaEventos.push(ev);
                } else {
                    // 2. NUEVA PLACA: ASIGNAR CELDA
                    int celdaLibre = -1;
                    for (int i = 0; i < TOTAL_CELDAS; i++) {
                        if (!celdas[i].ocupada) {
                            celdaLibre = i;
                            break;
                        }
                    }

                    if (celdaLibre != -1) {
                        celdas[celdaLibre].ocupada = true;
                        celdas[celdaLibre].placa = placa;
                        celdas[celdaLibre].marca = marca;
                        celdas[celdaLibre].color = color;
                        celdas[celdaLibre].horaIngreso = hora;

                        std::string nombreCelda = "C-" + std::to_string(celdaLibre + 1);
                        strcpy_s(ev.celda, nombreCelda.c_str());
                        ev.tipo = 1; // INGRESO
                        strcpy_s(ev.mensaje, "Ingreso registrado. Celda asignada.");
                        respuesta = "ASIGNADO|" + nombreCelda + "|INGRESO_EXITOSO\n";
                        colaEventos.push(ev);
                    } else {
                        ev.tipo = 3;
                        strcpy_s(ev.celda, "NINGUNA");
                        strcpy_s(ev.mensaje, "Parqueadero lleno.");
                        respuesta = "RECHAZADO|PARQUEADERO_LLENO\n";
                        colaEventos.push(ev);
                    }
                }
            }
            send(clienteSock, respuesta.c_str(), (int)respuesta.length(), 0);
        }
        closesocket(clienteSock);
    }
    closesocket(servidor_socket);
    WSACleanup();
}
