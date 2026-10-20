<p align="center">
  <img src="parqueadero.png" alt="SOMALU - Sistema de control de parqueadero" width="100%">
</p>

# Sistema Distribuido de Gestión de Parqueadero (C++ y Python vía SWIG)

## 1. Descripción General del Proyecto

El **Sistema Distribuido de Gestión de Parqueadero** es una solución de software de alta concurrencia e hibridación tecnológica, diseñada para automatizar, monitorear y administrar en tiempo real un recinto de estacionamiento con capacidad para **20 plazas**.

El sistema implementa una **arquitectura distribuida cliente-servidor** utilizando protocolos de red de bajo nivel (Sockets TCP/IP en C++), combinada con una capa de visualización interactiva en **Python**. La comunicación entre la lógica de control nativa y la interfaz gráfica se logra mediante la generación de un enlace de **librería dinámica** utilizando **SWIG** (*Simplified Wrapper and Interface Generator*), cumpliendo con estándares de desarrollo de software industrial y modularidad.

## 2. Arquitectura y Componentes del Sistema

La solución se divide en tres componentes principales que operan de forma cooperativa:

### 2.1 Módulo Servidor y Núcleo de Lógica (`parqueadero.cpp` / `parqueadero.h`)
* **Gestión de Concurrencia:** Utiliza hilos independientes (`std::thread`) y exclusión mutua (`std::mutex`) para garantizar la seguridad de los datos ante peticiones simultáneas de red.
* **Servidor de Sockets TCP:** Inicializa un socket pasivo en el puerto `8080` (utilizando la API WinSock2), procesando tramas de texto con el formato `MATRICULA|MARCA|COLOR|HORA`.
* **Lógica de Negocio Automática:**
  * *Ingreso de Vehículos:* Si la matrícula entrante no existe en el sistema, busca la primera celda libre (de C-1 a C-20), registra sus metadatos y emite una señal de asignación con su respectiva estampa de tiempo.
  * *Liberación por Matrícula Duplicada:* Si la placa ya se encuentra estacionada, el sistema la interpreta como una solicitud de salida, libera la plaza correspondiente y actualiza el estado.
  * *Control de Capacidad:* Si las 20 celdas están ocupadas, rechaza inteligentemente las nuevas solicitudes notificando estado de lleno.
* **Cola de Eventos:** Almacena los eventos generados en una estructura de cola segura (`std::queue`) para que la interfaz gráfica en Python pueda consultarlos mediante sondeos asíncronos no bloqueantes.

### 2.2 Módulo Cliente Generador de Tráfico (`cliente_generador.cpp`)
* Simula un proceso externo independiente que actúa como cliente TCP.
* Genera de forma pseudoaleatoria matrículas vehiculares estándar (3 letras mayúsculas y 3 dígitos numéricos, ej: `ABC123`), marcas comerciales y colores.
* Implementa un **pool de placas activas** con un factor de probabilidad del 40% para simular salidas de vehículos reintentando placas ya registradas, forzando así la liberación de celdas en el servidor.
* Transmite las tramas de datos de forma continua con intervalos temporales aleatorios de entre 2 y 5 segundos.

### 2.3 Módulo de Visualización Gráfica (`visualizador.py`)
* Desarrollado en **Python** utilizando **Tkinter** para una interfaz moderna y de bajo consumo de recursos (tema oscuro optimizado).
* Importa directamente la librería dinámica compilada por SWIG (`import parqueadero`), la cual expone funciones nativas de C++ (`IniciarServidor`, `HayEvento`, `ObtenerEvento`, `DetenerServidor`).
* Actualiza dinámicamente una matriz visual de 20 tarjetas correspondientes a las celdas del parqueadero y alimenta un registro en tiempo real (*Log de Eventos*) mediante un bucle de sondeo no bloqueante de 100ms (`self.after`).

## 3. Diagrama Arquitectónico y de Flujo

```text
+---------------------------------------+
|  Cliente Generador de Tráfico (C++)   |
|  - Genera placas aleatorias           |
|  - Simula ingresos y salidas          |
+---------------------------------------+
                    |
                    | Trama TCP/IP: PLACA|MARCA|COLOR|HORA
                    v (Puerto 8080)
+---------------------------------------+
|  Motor Servidor de Parqueadero (C++)  |
|  - Control por Mutex e Hilos          |
|  - Asignación de 20 Celdas            |
|  - Cola de Eventos                    |
+---------------------------------------+
                    |
                    | Enlace de Librería Dinámica (SWIG)
                    v
+---------------------------------------+
|  Interfaz Gráfica de Usuario (Python) |
|  - Visualizador en Tkinter            |
|  - Renderizado de Plazas en Tiempo Real|
+---------------------------------------+
```

## 4. Requisitos del Sistema y Dependencias

* **Compilador C++:** Entorno compatible con C++17 (MSVC en Visual Studio, MinGW-w64 o GCC).
* **Entorno Python:** Python 3.8 o superior con soporte para bibliotecas estándar de GUI.
* **Herramienta SWIG:** Instalada y registrada en las variables de entorno (*PATH*).
* **Librerías del Sistema Operativo:** Winsock2 (`ws2_32.lib`) para la gestión de sockets en redes Windows.

## 5. Guía de Instalación, Compilación y Ejecución

Siga los pasos a continuación para poner en marcha el sistema distribuido completo:

### Paso 1: Generación de la Librería Dinámica mediante SWIG
Ejecute el script de configuración para compilar las interfaces de C++ hacia Python:
```bash
python setup.py build_ext --inplace
```
*(Esto generará los archivos de enlace binario necesarios junto con el módulo de interfaz `parqueadero.py`)*.

### Paso 2: Ejecución del Módulo Visualizador (Servidor Central)
Inicie la aplicación gráfica de Python. Esta acción arrancará automáticamente el socket del servidor C++ en segundo plano:
```bash
python visualizador.py
```

### Paso 3: Ejecución del Cliente de Tráfico Vehicular
En una terminal independiente, compile el código fuente del cliente y ejecútelo para comenzar la transmisión:
```bash
g++ cliente_generador.cpp -o cliente_generador.exe -lws2_32
./cliente_generador.exe
```
