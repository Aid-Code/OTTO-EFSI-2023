#include <SPI.h>
#include "TimerOne.h"
#include <RF24.h>
#include <Servo.h>

#define SERVO1 10
#define SERVO2 9
#define SERVO3 6
#define SERVO4 3

#define BOT 2

#define TRIG 4
#define ECHO 5

#define INIT 0
#define JOYSTICK 1
#define ULTRASONICO 2

#define STANDBY   0
#define CHECK     1
#define RELEASE   2

int mSBot;

int estadoBoton;
int flagBot;
int tRebote = 15;

RF24 radio(7, 8); // CE, CSN

long t;
long d;

int x;
int y;

char dato [32] = "";

const byte identificacion[6] = "00001";

int modo = 0;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
//__________________________________________________________________________________________________

void setup() {

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(SERVO1, OUTPUT);
  pinMode(SERVO2, OUTPUT);
  pinMode(SERVO3, OUTPUT);
  pinMode(SERVO4, OUTPUT);

  pinMode(BOT, INPUT_PULLUP);

  servo1.attach(SERVO1);
  servo1.attach(SERVO2);
  servo2.attach(SERVO3);
  servo2.attach(SERVO4);

  Serial.begin(9600);
  radio.begin();

  radio.openReadingPipe(0, identificacion);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  Timer1.initialize(1000);
  Timer1.attachInterrupt(ISR_TIMER);
}
//__________________________________________________________________________________________________

void loop() {
  maquinaBoton();
  recibirComunicacion();
  leerUS();

  Serial.println("Radio: "+String(radio.available()));
  Serial.println("Bot:" + String(flagBot));
  Serial.println("X:" + String(x));
  Serial.println("Y:" + String(y));
  Serial.println("d:" + String(d));
  Serial.println("Modo:" + String(modo));
  Serial.println("");

  switch (modo) {
    case INIT:
      if (flagBot == 1) {
        modo = JOYSTICK;
        flagBot = 0;
      }
      break;

    case JOYSTICK:
      if (flagBot == 1) {
        modo = ULTRASONICO;
        flagBot = 0;
      }

      if (x > 900) {
        //derecha
        Serial.println("Derecha");
      }

      if (x <= 100) {
        //izquierda
        Serial.println("Izquierda");
      }

      if (y > 900) {
        //adelante
        Serial.println("Adelante");
      }

      if (y <= 100) {
        //atrás
        Serial.println("Atrás");
      }

      if (x <= 900 && x > 100 && y <= 900 && y > 100) {
        //quieto(idle)?
        Serial.println("Idle");
      }

      break;

    case ULTRASONICO:
      if (flagBot == 1) {
        modo = JOYSTICK;
        Serial.println("Modo ultrasonico");
        flagBot = 0;
      }

      if (d > 10) {
        //Hacer que OTTO gire
        Serial.println("Giro");
      }
      if (d <= 10) {
        //adelante
        Serial.println("Adelante");
      }
      break;
  }
  delay(500);
}

//__________________________________________________________________________________________________

void leerUS(void) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10); //Enviamos un pulso de 10us
  digitalWrite(TRIG, LOW);

  t = pulseIn(ECHO, HIGH); //obtenemos el ancho del pulso
  d = t / 59; //mapeamos el tiempo a una distancia en cm

  Serial.print("Distancia: " + String(d) + "cm");
  Serial.println("");
}
//__________________________________________________________________________________________________

void recibirComunicacion (void) {
  if (radio.available()) {

    //char dato[32] = "";
    //String dato = "";


    radio.read(&dato, sizeof(dato));
    
    String texto = String(dato);
   // Serial.println(texto);

    //String texto = "X1022-Y944#";

    int inicioX = texto.indexOf('X');
    int inicioY = texto.indexOf('Y');
    int fin = texto.indexOf('#');
    int guion = texto.indexOf('-');

    /*for (int i = 0; i < strlen(texto); i++) {
      if (texto[i] == 'X') {
        int inicioX = i;  // Guardamos el índice donde se encontró el carácter
        break;  // Salimos del bucle una vez que encontramos el carácter
      }
      }

      for (int j = 0; j < strlen(texto); j++) {
      if (texto[j] == 'Y') {
        int inicioY = j;  // Guardamos el índice donde se encontró el carácter
        break;  // Salimos del bucle una vez que encontramos el carácter
      }
      }

      for (int k = 0; k < strlen(texto); k++) {
      if (texto[k] == '-') {
        int guion = k;  // Guardamos el índice donde se encontró el carácter
        break;  // Salimos del bucle una vez que encontramos el carácter
      }
      }

      for (int l = 0; l < strlen(texto); l++) {
      if (texto[l] == '#') {
        int fin = l;  // Guardamos el índice donde se encontró el carácter
        break;  // Salimos del bucle una vez que encontramos el carácter
      }
      }*/

    String valX = texto.substring(inicioX + 1, guion);
    String valY = texto.substring(inicioY + 1, fin);

    x = valX.toInt();
    y = valY.toInt();

    Serial.println(valX + " " + valY);
  }
}
//__________________________________________________________________________________________________

void ISR_TIMER(void) {
  mSBot++;
}
//__________________________________________________________________________________________________

void maquinaBoton(void) {
  switch (estadoBoton) {
    case STANDBY:
      if (digitalRead(BOT) == LOW) {
        mSBot = 0;
        estadoBoton = CHECK;
      }
      break;

    case CHECK:
      if (mSBot <= tRebote && digitalRead(BOT) == HIGH) {
        estadoBoton = STANDBY;
      }

      if (mSBot >= tRebote && digitalRead(BOT) == LOW) {
        estadoBoton = RELEASE;
      }
      break;

    case RELEASE:
      if (digitalRead(BOT) == HIGH) {
        flagBot = 1;
        estadoBoton = STANDBY;
      }
      break;
  }
}
