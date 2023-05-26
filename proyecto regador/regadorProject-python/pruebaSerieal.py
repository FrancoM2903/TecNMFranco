import serial 
import time
import datetime 
import tkinter
from tkinter import *
import re
hora_actual = datetime.datetime.now().time()

while hora_actual.hour == 13 :


    serialArduino = serial.Serial("COM6",9600)
    time.sleep(1)

    while True:
        val = serialArduino.readline() # da valores numericos 
        cad = val.decode('ascii')
        print(cad)
        print("************")
