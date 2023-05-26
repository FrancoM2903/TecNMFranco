#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

// Incluímos la libreria externa para poder utilizarla
#include <LiquidCrystal_I2C.h> // Entre los símbolos <> buscará en la carpeta de librerías configurada

//variables globales 
const int PinTrig = 7;
const int PinEcho = 6;
const float Velson = 34000.0; //vel sonido cm/s
const int numLecturas = 10;
const float distancia100 = 2.38; //distancia100 = distanciaVacio-distanciaMuestra
const float distanciaVacio = 22.38;
float lecturas[numLecturas]; //array para lecturas y sacar media
int lecturaActual = 0;
float total = 0;
float media = 0;
bool primeraMedia = false; //la ponemos en true para condicion que muestre distancia 
void setup() {
//-------------------NIVEL DE AGUA-----------------//
Serial.begin(9600);
pinMode(PinTrig,OUTPUT);
pinMode(PinEcho,INPUT);

//------------------LCD----------------------------//
lcd.init();
lcd.backlight();
lcd.clear();
lcd.setCursor(0,0);

for (int i = 0; i < numLecturas; i++){
  lecturas[i] = 0 ;
}//nos aseguramos que el array este vacio 

}

void loop() {
//-------------------NIVEL DE AGUA-----------------//
total = total - lecturas[lecturaActual];
iniciarTrigger();
unsigned long tiempo = pulseIn(PinEcho,HIGH); //pulsein obtiene tiempo que tarda en cambiar entre estados para ver cuanto tarda en recibir la senial
float distancia = tiempo * 0.000001 * Velson / 2.0;//formula basica de distancia pero como el tiempo esta en microsegundos se usa conversion 1/100000
lecturas[lecturaActual] = distancia;
total = total + lecturas[lecturaActual];
lecturaActual++;

if (lecturaActual >= numLecturas){//comprobamos que sea el final de lecturas o final del array 
  primeraMedia = true;
  lecturaActual = 0;
}
//calculamos media 
media = total/numLecturas;
if (primeraMedia){//si es true primeraMedia
    float distanciaLleno = distanciaVacio - media;
    float cantidadLiquido = distanciaLleno *240/distancia100;
    int porcentaje = (int) (distanciaLleno * 100 / distanciaVacio);
    if (cantidadLiquido >= 1000){
      Serial.println("lleno");
      delay(500);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(String(cantidadLiquido)+" ml");
    }
    else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(String(cantidadLiquido)+" ml");
      Serial.println("vacio");
      delay(500);
    }
}
else {
  Serial.println("calculando");
}
}//fin loop 

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
