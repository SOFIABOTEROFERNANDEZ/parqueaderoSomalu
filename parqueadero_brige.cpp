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
