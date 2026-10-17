import ctypes
import os
import tkinter as tk
from tkinter import ttk

# Cargar la librería dinámica C++
DLL_PATH = os.path.abspath("parqueadero_bridge.dll")
dll = ctypes.CDLL(DLL_PATH)

# Definir prototipos de funciones
dll.IniciarModuloParqueadero.argtypes = [ctypes.c_int]
dll.IniciarModuloParqueadero.restype = ctypes.c_int
dll.HayNuevoEvento.restype = ctypes.c_int
dll.ObtenerSiguienteEvento.argtypes = [
    ctypes.POINTER(ctypes.c_int),
    ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p,
    ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p
]
dll.ObtenerSiguienteEvento.restype = ctypes.c_int
dll.DetenerModuloParqueadero.restype = None

class ParqueaderoVisualizer(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Sistema de Parqueadero - Visualizador GUI (Python + DLL C++)")
        self.geometry("980x660")
        self.configure(bg="#0f172a")

        self.TOTAL_CELDAS = 20
        self.celdas_widgets = {}

        # Iniciar Servidor socket a través de la DLL en el puerto 8080
        dll.IniciarModuloParqueadero(8080)

        self.crear_interfaz()
        self.protocol("WM_DELETE_WINDOW", self.al_cerrar)

        # Consultar la DLL cada 100ms
        self.after(100, self.verificar_eventos_dll)
