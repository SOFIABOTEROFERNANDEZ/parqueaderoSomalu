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

def crear_interfaz(self):
        header = tk.Frame(self, bg="#1e293b", height=60, padx=20, pady=10)
        header.pack(fill="x")

        tk.Label(header, text="CONTROL DE PARQUEADERO (20 CELDAS)", 
                 font=("Segoe UI", 15, "bold"), fg="#f8fafc", bg="#1e293b").pack(side="left")
        tk.Label(header, text="● DLL C++ ACTIVA", font=("Consolas", 10, "bold"), 
                 fg="#10b981", bg="#1e293b").pack(side="right")

        main_box = tk.Frame(self, bg="#0f172a", padx=15, pady=15)
        main_box.pack(fill="both", expand=True)

        # Panel de Celdas
        left_panel = tk.LabelFrame(main_box, text=" Estado de Celdas ", font=("Segoe UI", 11, "bold"), 
                                   fg="#38bdf8", bg="#0f172a", padx=10, pady=10)
        left_panel.pack(side="left", fill="both", expand=True, padx=(0, 10))

        grid_frame = tk.Frame(left_panel, bg="#0f172a")
        grid_frame.pack(fill="both", expand=True)
