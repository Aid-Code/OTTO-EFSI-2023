#include <SPI.h>
#include <RF24.h>

#define TRIG 4
#define ECHO 5

#define INIT 0
#define JOYSTICK 1
#define ULTRASONICO 2

RF24 radio(7, 8); // CE, CSN

long t;
long d;

char texto [32] = "";

const byte identificacion[6] = "00001";

int modo;

void setup() {

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(9600);
  radio.begin();

  radio.openReadingPipe(0, identificacion);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

}

void loop() {
  switch(modo){
    case INIT:
      modo=JOYSTICK;
      break;

    case JOYSTICK:
      recibirComunicacion();
      // Poner ifs que dependiendo del "texto" recibido OTTO vaya adelante, atrás, izquierda o derecha.
      break;

    case ULTRASONICO:
      if(d>10){
        //Hacer que OTTO gire.
      }
      if(d<=10){
        //Hacer que OTTO vaya hacia adelante. 
      }
      break;
  }

}

void leerUS(void){
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10); //Enviamos un pulso de 10us
  digitalWrite(TRIG, LOW);

  t = pulseIn(ECHO, HIGH); //obtenemos el ancho del pulso
  d = t / 59; //mapeamos el tiempo a una distancia en cm

  Serial.print("Distancia: " + String(d) + "cm");
  Serial.println("");
}

void recibirComunicacion (void){
  if (radio.available()) {
   texto = ""; 

  radio.read(&texto, sizeof(texto));

  Serial.println(texto);
}
