#include <LiquidCrystal_I2C.h>

// Incluímos la libreria externa para poder utilizarla
#include <LiquidCrystal_I2C.h> // Entre los símbolos <> buscará en la carpeta de librerías configurada
//--CONFIGURACION DE LCD  CON I2C--//
LiquidCrystal_I2C lcd(0x27,16,2); // si no te sale con esta direccion  puedes usar (0x3f,16,2) || (0x27,16,2)  ||(0x20,16,2)  

 
// Definimos las constantes
//------------------------NIVEL DE AGUA---------------------------// 
// Configuramos los pines del sensor Trigger y Echo
const int PinTrig = 7;
const int PinEcho = 6;
 
// Constante velocidad sonido en cm/s
const float VelSon = 34000.0;
 
// Número de muestras
const int numLecturas = 10;
 
// Distancia a los 100 ml y vacío//20
const float distancia100 = 2.38;//OJO ESTE VALOR ES DE LA ALTURA DEL LIQUIDO ES DECIR distancia vacio -lo que midio al tener 100 ml 
const float distanciaVacio = 22.38;
//const float distanciaml = (distanciaVacio - distancia100)/100;
 
float lecturas[numLecturas]; // Array para almacenar lecturas
int lecturaActual = 0; // Lectura por la que vamos
float total = 0; // Total de las que llevamos
float media = 0; // Media de las medidas
bool primeraMedia = false; // Para saber que ya hemos calculado por lo menos una

//-----------------------------CAUDALIMETRO---------------------------//
// Definir el pin utilizado para leer el pulso del caudalímetro
const byte pinCaudalimetro = 2;
// Volumen del tanque en mililitros
const float volumenTanque = 10000;
// Variables para el cálculo del caudal y el volumen gastado
volatile byte pulsos;
float caudal, volumenGastado;
// Variables para el cálculo del tiempo transcurrido
unsigned long tiempoAnterior, tiempoActual, tiempoTranscurrido; 
void setup()
{
  //CONFIGURACION LCD //
    lcd.init();
  lcd.backlight();
  lcd.clear();
 lcd.setCursor(0,0);
  //----------------------------------NIVEL DE AGUA--------------------------------//
  // Iniciamos el monitor serie para mostrar el resultado
  Serial.begin(9600);
  // Ponemos el pin Trig en modo salida
  pinMode(PinTrig, OUTPUT);
  // Ponemos el pin Echo en modo entrada
  pinMode(PinEcho, INPUT);
 
  // Inicializamos el array
  for (int i = 0; i < numLecturas; i++)
  {
    lecturas[i] = 0;
  }
 
  // Configuramos las filas y las columnas del LCD en este caso 16 columnas y 2 filas
  //lcd.begin(COLS, ROWS);


  //----------------------------CAUDALIMETRO-------------------------//
  // Inicializar el pin del caudalímetro como entrada
  pinMode(pinCaudalimetro, INPUT);
  // Configurar la interrupción en el pin del caudalímetro
  attachInterrupt(digitalPinToInterrupt(pinCaudalimetro), contarPulso, FALLING);
  // Inicializar el contador de pulsos
  pulsos = 0;
  // Iniciar la comunicación serial
  Serial.begin(9600);
  // Inicializar el tiempo anterior con el valor actual
  tiempoAnterior = millis();
  // Inicializar el volumen gastado en cero
  volumenGastado = 0;

}
void loop()
{

  //------------------------------------NIVEL DEL AGUA-------------------------------//
  // Eliminamos la última medida
  total = total - lecturas[lecturaActual];
 
  iniciarTrigger();
 
  // La función pulseIn obtiene el tiempo que tarda en cambiar entre estados, en este caso a HIGH
  unsigned long tiempo = pulseIn(PinEcho, HIGH);
 
  // Obtenemos la distancia en cm, hay que convertir el tiempo en segudos ya que está en microsegundos
  // por eso se multiplica por 0.000001
  float distancia = tiempo * 0.000001 * VelSon / 2.0;
 
  // Almacenamos la distancia en el array
  lecturas[lecturaActual] = distancia;
 
  // Añadimos la lectura al total
  total = total + lecturas[lecturaActual];
 
  // Avanzamos a la siguiente posición del array
  lecturaActual = lecturaActual + 1;
 
  // Comprobamos si hemos llegado al final del array
  if (lecturaActual >= numLecturas)
  {
    primeraMedia = true;
    lecturaActual = 0;
  }
 
  // Calculamos la media
  media = total / numLecturas;
 
  // Solo mostramos si hemos calculado por lo menos una media
  if (primeraMedia)
  {
    float distanciaLleno = distanciaVacio - media;
    float cantidadLiquido = distanciaLleno  *240/distancia100;
    int porcentaje = (int) (distanciaLleno * 100 / distanciaVacio);
 
    // Mostramos en la pantalla LCD
    //lcd.clear();
    // Cantidada de líquido
   // lcd.setCursor(0, 0);
    //lcd.print(String(cantidadLiquido) + " ml");
 
    // Porcentaje
   // lcd.setCursor(0, 1);
    //lcd.print(String(porcentaje) + " %");
 
    Serial.print(media);
    Serial.println(" cm");

    Serial.print(cantidadLiquido);
    Serial.println(" ml");
    Serial.print(porcentaje);
    Serial.println(" %");

    //impresion lcd
    //lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("d="+String(media)+"cm"+"|V="+String(cantidadLiquido)+"ml");
    lcd.setCursor(0,1);

  }
  else
  {
    //lcd.setCursor(0, 0);
    //lcd.print("Calculando: " + String(lecturaActual));
    Serial.print("Calculando..... ");
    Serial.println(lecturaActual);
    lcd.setCursor(0,0);
    lcd.print("Calculando....");
  }
 
  delay(500);

  //------------------------------CAUDALIMETRO--------------------------------//
// Calcular el tiempo transcurrido desde la última medición
  tiempoActual = millis();
  tiempoTranscurrido = tiempoActual - tiempoAnterior;
  // Calcular el caudal en litros por minuto
  caudal = pulsos / 7.5;
  // Calcular el volumen gastado en mililitros
  volumenGastado += (caudal * tiempoTranscurrido / 60000.0);
  // Actualizar el tiempo anterior con el valor actual
  tiempoAnterior = tiempoActual;
  // Mostrar el caudal y el volumen gastado en la consola serial
  Serial.print("Caudal: ");
  Serial.print(caudal);
  Serial.print(" L/min");
  Serial.print("   Volumen gastado: ");
  Serial.print(volumenGastado);
  Serial.println(" ml");

  //impresion LCD--//
  lcd.setCursor(0,1);
  lcd.print("C:"+String(caudal)+"L/m"+"|V:"+String(volumenGastado)+"ml");
  // Reiniciar el contador de pulsos
  pulsos = 0;
  // Esperar MEDIO segundo antes de realizar la próxima medición
  delay(500);

}

//===============================METODOS======================================//

//-----------------------NIVEL DE AGUA-----------------------------------//
 
// Método que inicia la secuencia del Trigger para comenzar a medir
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

//-------------------------CAUDALIMETRO-------------------------------//

void contarPulso() {
  // Incrementar el contador de pulsos
  pulsos++;
}
