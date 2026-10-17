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
