<p align="center">
  <img src="parqueadero.png" alt="SOMALU - Sistema de control de parqueadero" width="100%">
</p>


> Una solución híbrida de alto rendimiento para la simulación, monitoreo y gestión automatizada de un parqueadero de **20 celdas**, combinando la velocidad de procesamiento en **C++** con la flexibilidad de una interfaz gráfica moderna en **Python (Tkinter)**.

---

## 📑 Tabla de Contenidos
1. [Descripción General](#-descripción-general)
2. [Arquitectura del Sistema](#-arquitectura-del-sistema)
3. [Estructura del Proyecto](#-estructura-del-proyecto)
4. [Especificaciones Técnicas](#-specificaciones-técnicas)
5. [Requisitos Previos y Dependencias](#-requisitos-previos-y-dependencias)
6. [Guía de Instalación y Configuración](#-guía-de-instalación-y-configuración)
7. [Manual de Operación de la Interfaz](#-manual-de-operación-de-la-interfaz)
8. [Manejo de Errores y Solución de Problemas](#-manejo-de-errores-y-solución-de-problemas)
9. [Créditos y Contribución](#-créditos-y-contribución)

---

## 🌟 Descripción General

Este proyecto académico e industrial implementa un sistema cliente-servidor embebido de control de acceso vehicular. El núcleo del sistema corre bajo una lógica de bajo nivel optimizada en **C++**, encargada de administrar las transacciones de entrada, salida, asignación dinámica de celdas (del 1 al 20) y persistencia temporal de estados. 

Por su parte, la capa visual implementada en **Python** se comunica en tiempo real con este motor a través de una librería de enlace dinámico (`.dll`) y sockets de red (`WinSock2`), ofreciendo al operador un panel intuitivo con indicadores visuales de disponibilidad y una consola de eventos cronológica.

---

## 🏗️ Arquitectura del Sistema

La arquitectura sigue un patrón desacoplado donde la lógica pesada y de red reside en el backend nativo:

```text
+-----------------------+          Carga y llamadas          +-------------------------+
|    visualizador.py    | ---------------------------------> | parqueadero_bridge.dll  |
|  (Python + Tkinter)   | <--------------------------------- |      (Motor en C++)     |
+-----------------------+       Sockets / WinSock2           +-------------------------+
```

* **Backend (C++):** Administra el ciclo de vida de los vehículos, valida restricciones de cupo máximo (20 espacios) y procesa las tramas de comandos enviadas a través del protocolo de red local.
* **Frontend (Python):** Utiliza el módulo nativo `ctypes` para interactuar con las funciones exportadas por la DLL y ejecuta un bucle de sondeo (`polling` asíncrono con `after`) cada `100 ms` para refrescar los componentes gráficos sin bloquear el hilo principal de la interfaz.

---

## 📋 Estructura del Repositorio

La distribución de los archivos esenciales en el directorio de trabajo es la siguiente:

```text
.
├── parqueadero_bridge.dll    # Librería dinámica compilada (Binario C++)
├── visualizador.py           # Script principal de la Interfaz Gráfica (Python)
└── README.md                 # Documentación y manual técnico del sistema
```

---

## ⚙️ Especificaciones Técnicas

* **Capacidad Máxima:** 20 celdas numeradas secuencialmente del `1` al `20`.
* **Protocolo de Comunicación:** Sockets TCP/IP locales a través del puerto por defecto `8080`.
* **Codificación de Datos:** Tramas de texto plano estructuradas para intercambio rápido de comandos (Ingreso, Salida, Estado General).
* **Frecuencia de Actualización:** `100 milisegundos` en el cliente Python para la sincronización del estado de las celdas.

---

## 📦 Requisitos Previos y Dependencias

Antes de poner en marcha la aplicación, asegúrate de cumplir con los siguientes requisitos en tu entorno de desarrollo:

1. **Intérprete de Python:** Versión `3.8` o superior instalada y añadida a las variables de entorno del sistema (`PATH`).
2. **Entorno Operativo:** Sistema operativo Windows (debido a la dependencia nativa de la librería compilada con llamadas a `WinSock2` y formato `.dll`).
3. **Librerías Nativas de Python:** El proyecto utiliza exclusivamente módulos incluidos en la librería estándar (`tkinter`, `ctypes`, `threading`, `time`), por lo que **no requiere** la instalación de paquetes externos mediante `pip`.

---

## 🚀 Guía de Instalación y Ejecución

Sigue estos sencillos pasos para clonar, preparar y ejecutar el sistema en tu máquina local:

1. **Clonar o descargar el repositorio:**
   Ubícate en la carpeta donde deseas alojar el proyecto.

2. **Verificar archivos binarios:**
   Comprueba que el archivo `parqueadero_bridge.dll` se encuentre ubicado exactamente en la misma ruta que el script `visualizador.py`.

3. **Ejecutar la Interfaz Gráfica:**
   Abre una terminal (CMD o PowerShell) en la ruta del proyecto y ejecuta el siguiente comando:
   ```bash
   python visualizador.py
   ```

4. **Verificación de Conexión:**
   Una vez lanzado el script, la ventana principal de Tkinter se abrirá mostrando el estado inicial del parqueadero. El log de eventos indicará si la conexión con la DLL se estableció de manera exitosa.

---

## 🎛️ Manual de Operación de la Interfaz

La interfaz gráfica ha sido diseñada bajo principios de usabilidad para operadores de sistemas de control:

* **Panel de Celdas (Grid 4x5):** 
  * Las celdas disponibles se muestran con indicadores visuales distintivos (por ejemplo, tonos verdes).
  * Las celdas ocupadas reflejan el estado activo y muestran la información básica del vehículo asociado (tono rojo/gris oscuro).
* **Consola de Historial (Log):**
  * Ubicada en la sección inferior o lateral de la ventana, registra cronológicamente cada ingreso de placa, liberación de celda y advertencia del sistema con marcas de tiempo precisas.
* **Controles de Acceso:**
  * Permiten enviar las solicitudes de registro al backend en C++ de forma inmediata con un solo clic.

---

## 🛠️ Manejo de Errores y Solución de Problemas

* **Error: `FileNotFoundError` o fallo al cargar la DLL**
  * *Causa:* Python no logra encontrar el archivo `parqueadero_bridge.dll` en el directorio actual.
  * *Solución:* Asegúrate de ejecutar el script `visualizador.py` estando posicionado exactamente en la carpeta raíz del proyecto.
* **Error de Sockets / Puerto Ocupado**
  * *Causa:* El puerto `8080` está siendo utilizado por otra aplicación en segundo plano.
  * *Solución:* Cierra procesos residuales de ejecuciones anteriores de C++ o cambia temporalmente el puerto de escucha en la configuración del backend.
