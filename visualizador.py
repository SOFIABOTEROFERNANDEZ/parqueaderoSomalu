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
       for i in range(self.TOTAL_CELDAS):
            row, col = i // 5, i % 5
            card = tk.Frame(grid_frame, bg="#1e293b", relief="groove", bd=2, padx=5, pady=5)
            card.grid(row=row, column=col, padx=4, pady=4, sticky="nsew")
            grid_frame.grid_columnconfigure(col, weight=1)
            grid_frame.grid_rowconfigure(row, weight=1)

            tk.Label(card, text=f"C-{i+1}", font=("Consolas", 9, "bold"), fg="#94a3b8", bg="#1e293b").pack(anchor="w")
            lbl_placa = tk.Label(card, text="LIBRE", font=("Consolas", 11, "bold"), fg="#64748b", bg="#1e293b")
            lbl_placa.pack(pady=4)
            lbl_hora = tk.Label(card, text="--:--:--", font=("Segoe UI", 8), fg="#64748b", bg="#1e293b")
            lbl_hora.pack(anchor="e")

            self.celdas_widgets[i] = {"frame": card, "placa": lbl_placa, "hora": lbl_hora}

        # Panel de Eventos (Historial)
        right_panel = tk.LabelFrame(main_box, text=" Registro de Eventos ", font=("Segoe UI", 11, "bold"), 
                                    fg="#a855f7", bg="#0f172a", width=340, padx=10, pady=10)
        right_panel.pack(side="right", fill="both", padx=(10, 0))

        self.log_list = tk.Listbox(right_panel, bg="#020617", fg="#e2e8f0", font=("Consolas", 9), width=38)
        self.log_list.pack(fill="both", expand=True)
cc
