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
