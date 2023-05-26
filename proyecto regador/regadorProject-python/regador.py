""" ==================== paquetes  ==================== """
from tkinter import *
import re
import tkinter as tk
from PIL import ImageTk, Image

""" ============================================ VARIABLES ===================================================="""
root = tk.Tk()

img1 = ImageTk.PhotoImage(Image.open("img/automata.jpeg"))
img2 = ImageTk.PhotoImage(Image.open("img/modelo1.jpg"))
colorbackgound='#19A7CE'
res=""
""" ==================== METODO CAMBIAR MENSAJE ==================== """
""" ==================== ESTE METODO SE ENCARGARA DE ASIGNAR LAS FUNCIONES QUE SE EFECTARAN CUANDO SE HAGA CLIC EN EL BOTON DE VALIDAR ==================== """
def on_click(texto):
   lbl_respuesta["text"] = "texto"
   print(texto)

""" ==================== La clase My_App tiene la funcion de crear la app y sus componenetes ==================== """
#total de la ventana son 830*630
#para la seccion1 830*315
seccion11 = tk.Frame(root,background=colorbackgound)
seccion12 = tk.Frame(root,background=colorbackgound)
seccion13 = tk.Frame(root,background=colorbackgound)
seccion14 = tk.Frame(root,background=colorbackgound)

# para la seccion2 415*315
seccion2 = tk.Frame(root,background=colorbackgound)
# para la seccion3 415*315
seccion3 = tk.Frame(root,background=colorbackgound)

# Ubicar los marcos en la ventana
seccion11.grid(row=0, column=0, columnspan=6)
seccion12.grid(row=1, column=0, columnspan=3)
seccion13.grid(row=1, column=3, columnspan=3)
seccion14.grid(row=2, column=0, columnspan=6)
seccion2.grid(row=3, column=0,columnspan=3)
seccion3.grid(row=3, column=3,columnspan=3)

        # Create widgets
        #tabla de 6x6
        #seccion11
lbl_titulo = tk.Label(seccion11, text="PROYECTO: Regador",font=35,background=colorbackgound)
lbl_titulo.pack(pady=15,padx=0)
#seccion12
lbl_automata = tk.Label(seccion12, text="Automata",font=25,background=colorbackgound)
lbl_automata.pack(pady=15,padx=0)
img_automata = Label(seccion12, image=img1,background=colorbackgound)
img_automata.pack()
#seccion13
lbl_problematica = tk.Label(seccion13, text="Modelo de la problematica",font=25,background=colorbackgound)
lbl_problematica.pack(pady=15,padx=0)
img_modelo = Label(seccion13, image=img2,background=colorbackgound)
img_modelo.pack()
#seccion14
lbl_alfabeto = tk.Label(seccion14, text="ALFABETO ∑={0,1} ",font=25,background=colorbackgound)
lbl_alfabeto.pack(pady=15,padx=0)
lbl_lenguaje = tk.Label(seccion14, text="L={Son las cadenas sobre el alfabeto ∑={0,1} de longitud múltiplo de 4, que no contiene la cadena vacía y que puede contener 0 ó 1´s seguidos o alternados}",font=25,background=colorbackgound)
lbl_lenguaje.pack(pady=15,padx=0)
lbl_ereg = tk.Label(seccion14, text="Expresión Regular { ( [ (0|1) (0|1) ] ( [ (1) (0|1) ] | (00) ) )+} ",font=25,background=colorbackgound)
lbl_ereg.pack(pady=15,padx=0)
#seccion3
lbl_rescartel = tk.Label(seccion3, text="RESULTADO",font=25,background=colorbackgound)
lbl_rescartel.pack(pady=15,padx=0)
lbl_respuesta = tk.Label(seccion3, text="", font=25,background=colorbackgound)
lbl_respuesta.pack(pady=15, padx=0)
# seccion2
L1 = Label(seccion2, text="INGRESE SU CADENA", font=25, background=colorbackgound)
L1.pack()
txt_cadena = tk.Entry(seccion2, bd=1, font=25, background=colorbackgound)
txt_cadena.pack()
btn_enviar = tk.Button(seccion2, text="Validar", font=25, background=colorbackgound, command=on_click(txt_cadena.get()))
btn_enviar.pack(pady=15, padx=0)

""" ==================== METODO VALIDAR CADENA ==================== """
""" ==================== La función validar_cadena() valida la cadena que se ingrese mediante la expresión regular. ==================== """
def validar_cadena(cadena):
    res=""
    patron = re.compile("((0|1)(0|1)(0|1)(0|1)(0|1))((0|1)(0|1)(0|1)(0|1)(0|1))*")
    if patron.fullmatch(
            cadena) is not None:  # ?fullmatch retorna none si no hay matcheo en cadena por ende si no es none la cadena es valida
        res="Cadena válida"
        return res
    else:
        if len(cadena) == 5:
            for letra in cadena:
                bandera_caracter_invalido = False
                while letra not in '01':
                    bandera_caracter_invalido = True
                    break
                if bandera_caracter_invalido == False:
                    res="La cadena cumple con las reglas"
                else:
                    res="La cadena contiene caracteres que no son 0 ni 1"
                    break
        else:
            res="La cadena a validar debe tener una longitud de 5"
        res="Cadena inválida: no cumple con las reglas"
        return res

""" ============================================ Ejecucion del proceso de creacion de la ventana ===================================================="""

root.title("Regador de agua")
root.iconbitmap("img/sprinkler.ico")
root.geometry('1440x1050+0+0')
root['background']=colorbackgound
root.config(relief="groove", bd=15)

root.resizable(0,0)
root.mainloop()

