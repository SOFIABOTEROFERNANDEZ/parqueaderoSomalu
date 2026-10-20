import tkinter as tk
import parqueadero # <-- Módulo C++ importado gracias a SWIG

class ParqueaderoVisualizer(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Sistema de Parqueadero - Visualizador GUI (Python + SWIG C++)")
        self.geometry("980x660")
        self.configure(bg="#0f172a")

        self.TOTAL_CELDAS = 20
        self.celdas_widgets = {}

        # Iniciar Servidor Socket C++ a través de SWIG
        parqueadero.IniciarServidor(8080)
        print("[INFO] Servidor C++ iniciado via SWIG en puerto 8080")

        self.crear_interfaz()
        self.protocol("WM_DELETE_WINDOW", self.al_cerrar)
        self.after(100, self.verificar_eventos_swig)

    def crear_interfaz(self):
        header = tk.Frame(self, bg="#1e293b", height=60, padx=20, pady=10)
        header.pack(fill="x")

        tk.Label(header, text="CONTROL DE PARQUEADERO (SWIG + C++)", 
                 font=("Segoe UI", 15, "bold"), fg="#f8fafc", bg="#1e293b").pack(side="left")
        tk.Label(header, text="● MODULO SWIG CONECTADO", font=("Consolas", 10, "bold"), 
                 fg="#10b981", bg="#1e293b").pack(side="right")

        main_box = tk.Frame(self, bg="#0f172a", padx=15, pady=15)
        main_box.pack(fill="both", expand=True)

        left_panel = tk.LabelFrame(main_box, text=" Estado de Celdas (20 Plazas) ", 
                                   font=("Segoe UI", 11, "bold"), fg="#38bdf8", bg="#0f172a", padx=10, pady=10)
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

        right_panel = tk.LabelFrame(main_box, text=" Registro de Eventos ", font=("Segoe UI", 11, "bold"), 
                                    fg="#a855f7", bg="#0f172a", width=340, padx=10, pady=10)
        right_panel.pack(side="right", fill="both", padx=(10, 0))

        self.log_list = tk.Listbox(right_panel, bg="#020617", fg="#e2e8f0", font=("Consolas", 9), width=38)
        self.log_list.pack(fill="both", expand=True)

    def verificar_eventos_swig(self):
        if parqueadero.HayEvento():
            ev = parqueadero.ObtenerEvento()

            # ev es un objeto C++ con atributos nativos en Python
            s_placa = ev.placa
            s_celda = ev.celda
            s_hora = ev.hora
            idx = int(s_celda.replace("C-", "")) - 1 if "C-" in s_celda else -1

            if ev.tipo == 1: # INGRESO
                self.log_list.insert(0, f"[{s_hora}] INGRESO: {s_placa} en {s_celda}")
                if 0 <= idx < self.TOTAL_CELDAS:
                    self.celdas_widgets[idx]["frame"].configure(bg="#064e3b")
                    self.celdas_widgets[idx]["placa"].configure(text=s_placa, fg="#34d399", bg="#064e3b")
                    self.celdas_widgets[idx]["hora"].configure(text=s_hora, fg="#a7f3d0", bg="#064e3b")

            elif ev.tipo == 2: # LIBERACION (Placa repetida)
                self.log_list.insert(0, f"[{s_hora}] LIBERADA: {s_placa} salio de {s_celda}")
                if 0 <= idx < self.TOTAL_CELDAS:
                    self.celdas_widgets[idx]["frame"].configure(bg="#1e293b")
                    self.celdas_widgets[idx]["placa"].configure(text="LIBRE", fg="#64748b", bg="#1e293b")
                    self.celdas_widgets[idx]["hora"].configure(text="--:--:--", fg="#64748b", bg="#1e293b")

        self.after(100, self.verificar_eventos_swig)

    def al_cerrar(self):
        parqueadero.DetenerServidor()
        self.destroy()

if __name__ == "__main__":
    app = ParqueaderoVisualizer()
    app.mainloop()