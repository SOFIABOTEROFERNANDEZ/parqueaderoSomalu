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
