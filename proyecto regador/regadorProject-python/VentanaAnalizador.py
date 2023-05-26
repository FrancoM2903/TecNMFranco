from tkinter import *
import re
import serial 
import time
import datetime 
hora_actual = datetime.datetime.now().time()


#Clase que contenga el analizador sjjsjsj
class AnalizadorRegador(Tk):

    def __init__(self): #Inicializar la ventana del analizador
        super().__init__()
        colorblanco = "#F7FCDF"
        self.errorLexico = False
        self.errorSintactico = False
        self.textoOutput = ""
        self.cadenasInvalidas = ""

        self.tokens = [             #Tipo de token , valores que tiene el token en expresion regular
            ('Agua', r'\s*(SI|Si|si|NO|No|no)\s*'),
            ('Sensor', r'\s*(HUMEDO|Humedo|humedo|SECO|Seco|seco)\s*'),
            ('Terminador', r'\s*;\s*')
        ]

        #Ventana
        self.title("ANALIZADOR LEXICO (REGADOR AUTOMATICO)") #Titulo
        self.config(bg = colorblanco)

        #Menu y opciones
        self.barramenu = Menu(self)
        self.config(menu = self.barramenu) #Configurar un menú
        self.menuarchivo = Menu(self.barramenu, tearoff = False) #Configurar nuevo menú de cascada
        self.menuarchivo.add_command(
            label = "Salir",                #Salir de la app
            command = self.destroy)
        self.barramenu.add_cascade(
            label = "Archivo",
            menu = self.menuarchivo)

        #Editor de Texto
        self.frameEditorTexto = Frame(self, bg = "gray")
        self.frameEditorTexto.pack(padx = 2, pady=2, side = LEFT)
        self.editorTexto = Text(self.frameEditorTexto, bg = "gray26", fg = "white", height = 15,
                                width = 40, font = ("Dhasis", 15))
        self.editorTexto.pack()
        self.editorTexto.focus()            #Editor Texto, ya seleciconado

        #Botones y output
        self.frameAnalisis = Frame(self, bg = colorblanco)
        self.frameAnalisis.pack(side = RIGHT)
        self.labelOutput = Label(self.frameAnalisis, text="Resultados\n\n\n", width = 40,
                                 bd = 1, height = 10, font = "Dhasis, 15") #, textalign = "LEFT"
        self.labelOutput.pack()         #Texto de salida según el botón presionado
        self.botonLexico = Button(self.frameAnalisis, text = "Analizar Lexico",  command = lambda:self.obtenerLexico(),
                                  font = ("Dhasis", 15,"bold"))
        self.botonLexico.pack()     #Botón que valida el lexico
        self.botonSintactico = Button(self.frameAnalisis, text="Analizar Sintactico", command = lambda:self.obtenerSintactico(),
                                      font = ("Dhasis", 15,"bold"))
        self.botonSintactico.pack() #Botón que valida el sintactico
        self.botonSerial = Button(self.frameAnalisis, text = "Obtener Cadena Serial", command = lambda : self.obtenerSerial(),
                                  font = ("Dhasis", 15,"bold"))
        self.botonSerial.pack()

    def obtenerSerial(self):
        #while hora_actual.hour == 13 :
        serialArduino = serial.Serial("COM6",9600)
        time.sleep(1)

        banCambio = True

        #while True : 
        val = serialArduino.readline() # da valores numericos 
        cad = val.decode('ascii')

        self.editorTexto.insert("end", cad)
        print (cad)
        print ("**********")


    def obtenerLexico(self):
        self.input = self.editorTexto.get("1.0", "end-1c")  #Recuperar el texto del editor
        print(self.input)#AQUI HICIMOS UN CAMBIO 
        tokens = self.validarLexico(self.input)     #Lista de tokens encontrados en el texto

        if not self.errorLexico:        #Imprimir tokens o errores
            self.textoOutput = f"Cadena Válida\nTokens:\n"
            puntero = 0
            multiplo3 = 3
            for token in tokens:
                if puntero == multiplo3 - 1:
                    self.textoOutput += f"{token}\n"
                    multiplo3 += multiplo3
                else:
                    self.textoOutput += str(token)
                puntero += 1
            self.labelOutput.config(text = self.textoOutput)
            print(f"{tokens} \n")
        else:
            self.labelOutput.config(text = f"Cadena inválida\nLas cadenas {self.cadenasInvalidas} no son válidas")

        self.errorLexico = False
        self.textoOutput = ""       #Limpiar variables
        self.cadenasInvalidas = ""


    def validarLexico(self, cadena):
        # Función para devolver los tokens o las cadenas no permitidas
        i = 0
        tokens = []
        invalida = ""
        ierror = -1

        while i < len(cadena):      #recorrido de toda la cadena
            match = None
            for tipo_token, valores_token in self.tokens:       #Buscar tokens en toda la cadena y añadirlos a lista
                regex = re.compile(valores_token)
                match = regex.match(cadena, i)
                if match:
                    tokens.append((tipo_token, match.group(0)))
                    i = match.end(0)
                    break
            if not match:           #Si hay palabras invalidas añadirlas a lista
                if i != 0 and self.errorLexico:
                    if ierror < i-1:
                        invalida += "\n"
                invalida += str(cadena[i])
                ierror = i
                i += 1
                self.errorLexico = True
        self.cadenasInvalidas = invalida
        return tokens               #retornar tokens validos encontrados en la cadena

    def obtenerSintactico (self):
        self.input = self.editorTexto.get("1.0", "end-1c")
        print(self.input)
        tokens = self.validarLexico(self.input)
        if not self.errorLexico:
            self.validarSintactico(tokens)
            if not self.errorSintactico:
                self.labelOutput.config(text = "Cadena Válida\nSe cumplen con las normas")
            else:
                self.labelOutput.config(text = f"Cadena inválida\n{self.textoOutput}")
        else:
            self.labelOutput.config(text = f"Cadena Inválida\nRevise su léxico\nSentencias con errores")
        self.errorLexico = False
        self.errorSintactico = False
        self.textoOutput = ""
        self.cadenasInvalidas = ""

    def validarSintactico(self, tokens):
        estructura_obtenida = ""
        er = r"(AguaSensorSensorTerminador)+"

        for tipo_token, valores_token in tokens:
            estructura_obtenida += str(tipo_token)

        regex = re.compile(er)
        match = regex.match(estructura_obtenida, 0)
        if match:
            print(f"La cadena es válida\nEstructura: \n{estructura_obtenida}")
        else:
            self.errorSintactico = True
            try:
                if tokens[0][0] == "Agua":
                    if len(tokens) > 1 and tokens[1][0] == "Sensor":
                        if len(tokens) > 2 and tokens[2][0] == "Sensor":
                            if len(tokens) > 3 and tokens[3][0] == "Terminador":
                                print("Algo malo pasó aqui")
                            else: self.textoOutput = f"Error: Se espera (';') al final de la sentencia"
                        else: self.textoOutput += f"Error: Se espera ('Humedo/Seco') como tercera instrucción"
                    else: self.textoOutput += f"Error: Se espera ('Humedo/Seco') como segunda instrucción"
                else: self.textoOutput += f"Error: Se espera ('Si/No') como primera instrucciónn"
            except:
                self.textoOutput = "Error, Ingresa al menos tres instrucciones y un terminador"

programa = AnalizadorRegador()
programa.mainloop()