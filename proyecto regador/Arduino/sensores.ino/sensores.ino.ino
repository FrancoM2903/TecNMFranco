#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
//variables globales 
//----------------------SUBSONICO-------------------------------//
const int PinTrig = 7;
const int PinEcho = 6;
const float Velson = 34000.0; //vel sonido cm/s
const int numLecturas = 1;
const float distancia100 = 2.38; //distancia100 = distanciaVacio-distanciaMuestra
const float distanciaVacio = 22.38;
float lecturas[numLecturas]; //array para lecturas y sacar media
int lecturaActual = 0;
float total = 0;
float media = 0;
bool primeraMedia = false; //la ponemos en true para condicion que muestre distancia 
//---------------------HUMEDAD--------------------------------------//
const int humedad = A0;
const int humedad2 = A1;
//------------------AUXILIARES CAMBIO-----------------------------//
String auxVal1 = "";
String auxVal2 = "";
String valor1 = "";
String valor2 = "";
//AGUA//
String auxVal3 = "";
String valor3 = "";

void setup (){

//------------------LCD----------------------------//
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
//----------------- AGUA----------------------------//
  Serial.begin(9600);
  pinMode(PinTrig,OUTPUT);
  pinMode(PinEcho,INPUT);     
//-----------------HUMEDAD--------------------------//
 pinMode(humedad,INPUT);
 //---------------CICLO MUESTRAS-------------------//
 for (int i = 0; i < numLecturas; i++){
  lecturas[i] = 0 ;
}//nos aseguramos que el array este vacio 

}

void loop(){
//-------------------NIVEL DE AGUA-----------------//
  total = total - lecturas[lecturaActual];
  iniciarTrigger();
  unsigned long tiempo = pulseIn(PinEcho,HIGH); //pulsein obtiene tiempo que tarda en cambiar entre estados para ver cuanto tarda en recibir la senial
  float distancia = tiempo * 0.000001 * Velson / 2.0;//formula basica de distancia pero como el tiempo esta en microsegundos se usa conversion 1/100000
  lecturas[lecturaActual] = distancia;
  total = total + lecturas[lecturaActual];
  lecturaActual++;
//------------MUESTRAS---------------//
  if (lecturaActual >= numLecturas){//comprobamos que sea el final de lecturas o final del array 
    primeraMedia = true;
    lecturaActual = 0;
}
//calculamos media PARA SEGUIR 
media = total/numLecturas;
//------------------CICLO DE MUESTRAS -----------------//
if (primeraMedia){//si es true primeraMedia
//-----------------NIVEL DE AGUA---------------------//
    float distanciaLleno = distanciaVacio - media;
    float cantidadLiquido = distanciaLleno *240/distancia100;
    int porcentaje = (int) (distanciaLleno * 100 / distanciaVacio);
    if (cantidadLiquido >= 1000){
      //Serial.println("lleno");
      valor3 = "si";
      delay(500);
      //lcd.clear();
      lcd.setCursor(0,1);
      lcd.print(String(cantidadLiquido)+" ml");
    }
    else {
      //lcd.clear();
      lcd.setCursor(0,1);
      lcd.print(String(cantidadLiquido)+" ml");
      //Serial.println("vacio");
      valor3 = "no";
      delay(500);
    }

    //-----------HUMEDAD----------------------//
    int valorSensor1 = analogRead(humedad);
    int valorSensor2 = analogRead(humedad2);
   if (valorSensor1 < 300){
    //lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("s1 hume");
    delay(500);
    valor1 = "humedo";

  }
  else {
    //lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("s1 seco");
    delay(500);
    valor1 = "seco";
  }
   if (valorSensor2 < 700){
    //lcd.clear();
    lcd.setCursor(9,0);
    lcd.print("s2 hume");
    delay(500);
    valor2 = "humedo";

  }
  else {
    //lcd.clear();
    lcd.setCursor(9,0);
    lcd.print("s2 seco");
    delay(500);
    valor2 = "seco";
  }
  //-------------------MANDA VALORES A SERIAL EN UN CAMBIO --------------//  
   if ((auxVal1 != valor1) || (auxVal2 != valor2) || (auxVal3 != valor3) ){//cuando cambien valores de los sensores se manda a serial TODO 
    auxVal1 = valor1;
    auxVal2 = valor2;
    auxVal3 = valor3;
    Serial.println(valor3  + valor1 +  valor2 + ";");
    //Serial.println(cantidadLiquido);
  } 
}//FIN PRIMER IF MUESTRAS
else {
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print("Calculando");
}//fin ciclo muestras 
}//FIN LOOP

void iniciarTrigger()
{
  // Ponemos el Triiger en estado bajo y esperamos 2 ms
  digitalWrite(PinTrig, LOW);
  delayMicroseconds(2);
 
  // Ponemos el pin Trigger a estado alto y esperamos 10 ms
  digitalWrite(PinTrig, HIGH);
  delayMicroseconds(10);
 
  // Comenzamos poniendo el pin Trigger en estado bajo
  digitalWrite(PinTrig, LOW);
}