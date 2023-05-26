#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

// Incluímos la libreria externa para poder utilizarla
#include <LiquidCrystal_I2C.h>

const int humedad = A0;
const int humedad2 = A1;
String auxVal1 = "";
String auxVal2 = "";
String valor1 = "";
String valor2 = "";



void setup() {
  Serial.begin(9600);
  pinMode(humedad,INPUT);

lcd.init();
lcd.backlight();
lcd.clear();
lcd.setCursor(0,0);

}

void loop() {
  int valorSensor1 = analogRead(humedad);
  int valorSensor2 = analogRead(humedad2);

  if (valorSensor1 < 300){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("sensor 1 humedo");
    delay(500);
    valor1 = "humedo";

  }
  else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("sensor 1 seco");
    delay(500);
    valor1 = "seco";
  }
   if (valorSensor2 < 700){
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("sensor 2 humedo");
    delay(500);
    valor2 = "humedo";

  }
  else {
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("sensor 2 seco");
    delay(500);
    valor2 = "seco";
  }


  if ((auxVal1 != valor1) || (auxVal2 != valor2) ){
    auxVal1 = valor1;
    Serial.println (valor1);
    auxVal2 = valor2;
    Serial.println (valor2);
  }
  Serial.println("//-----------------------------------------//");
  Serial.println("humedad 1 " + String(valorSensor1));
  delay(1000);
  Serial.println("humedad 2 "+ String(valorSensor2));
  delay(1000);

}
