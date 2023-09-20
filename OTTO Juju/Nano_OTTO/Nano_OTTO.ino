#include <Otto.h>
#include <SPI.h>
#include <RF24.h>

Otto Otto;

#define TRIG 4
#define ECHO 5

#define LeftLeg 2
#define RightLeg 3
#define LeftFoot 4
#define RightFoot 5
#define Buzzer  13

//Estados Maquina Control
#define INIT 0
#define JOYSTICK 1
#define ULTRASONICO 2

//Estados Maquina Movimiento
#define IDLE 0
#define ADELANTE 1
#define ATRAS 2
#define IZQUIERDA 3
#define DERECHA 4

RF24 radio(7, 8); // CE, CSN

long t;
long d;

char texto [32] = "";

const byte identificacion[6] = "00001";

int modo;
int actualState;

void setup() {

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(9600);
  radio.begin();

  radio.openReadingPipe(0, identificacion);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  Otto.init(LeftLeg, RightLeg, LeftFoot, RightFoot, true, Buzzer); //Set the servo pins and Buzzer pin
  Otto.sing(S_connection); //Otto wake up!
  Otto.home();
  actualState = IDLE; //Define el estado como "IDLE" --> Sin movimiento
  delay(50);
}

void loop() {
  switch (modo) {
    case INIT:
      modo = JOYSTICK;
      break;

    case JOYSTICK:
      recibirComunicacion();
      Movement(texto);
      break;

    case ULTRASONICO:
      if (d > 10) {
        Otto.turn(2, 1000, 1);
      }
      if (d <= 10) {
        Otto.walk(4, 2000, 1);
      }
      break;
  }

}

void leerUS(void) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10); //Enviamos un pulso de 10us
  digitalWrite(TRIG, LOW);

  t = pulseIn(ECHO, HIGH); //obtenemos el ancho del pulso
  d = t / 59; //mapeamos el tiempo a una distancia en cm

  Serial.print("Distancia: " + String(d) + "cm");
  Serial.println("");
}

void recibirComunicacion (void) {
  if (radio.available()) {
    texto = "";

    radio.read(&texto, sizeof(texto));

    Serial.println(texto);
  }
}

void Movement(String text)
{
  switch (actualState)
  {
    case IDLE:
      if (text == "adelante")
      {
        Otto.walk(4, 2000, 1); //Otto.walk(Cantidad de pasos, Tiempo, Direccion); --> Direccion; 1 = Adelante ; -1 = Atras
        actualState = ADELANTE;
      }
      if (text == "atras")
      {
        Otto.walk(4, 2000, -1);
        actualState = ATRAS;
      }
      if (text == "izquierda")
      {
        Otto.turn(2, 1000, 1); //Otto.turn(Cantidad de pasos, Tiempo, Direccion); --> Direccion; 1 = Izquierda ; -1 = Derecha
        actualState = IZQUIERDA;
      }
      if (text == "derecha")
      {
        Otto.turn(2, 1000, -1);
        actualState = DERECHA;
      }
      break;

    case ADELANTE:
      if (text == "idle")
      {
        Otto.walk(0, 1000, 1); //Defino 0 pasos como para no generar movimiento; No se si este comando funciona correctamente para lo que quiero hacer, hay que testearlo
        Otto.turn(0, 1000, 1);
        actualState = IDLE;
      }
      if (text == "atras")
      {
        Otto.walk(4, 2000, -1);
        actualState = ATRAS;
      }
      if (text == "izquierda")
      {
        Otto.turn(2, 1000, 1);
        actualState = IZQUIERDA;
      }
      if (text == "derecha")
      {
        Otto.turn(2, 1000, -1);
        actualState = DERECHA;
      }
      break;

    case ATRAS:
      if (text == "idle")
      {
        Otto.walk(0, 1000, 1);
        Otto.turn(0, 1000, 1);
        actualState = IDLE;
      }
      if (text == "adelante")
      {
        Otto.walk(4, 2000, 1);
        actualState = ADELANTE;
      }
      if (text == "izquierda")
      {
        Otto.turn(2, 1000, 1);
        actualState = IZQUIERDA;
      }
      if (text == "derecha")
      {
        Otto.turn(2, 1000, -1);
        actualState = DERECHA;
      }
      break;

    case IZQUIERDA:
      if (text == "idle")
      {
        Otto.walk(0, 1000, 1);
        Otto.turn(0, 1000, 1);
        actualState = IDLE;
      }
      if (text == "adelante")
      {
        Otto.turn(0, 1000, 1);
        Otto.walk(4, 2000, 1);
        actualState = ADELANTE;
      }
      if (text == "atras")
      {
        Otto.turn(0, 1000, 1);
        Otto.walk(4, 2000, -1);
        actualState = ATRAS;
      }
      if (text == "derecha")
      {
        Otto.turn(2, 1000, -1);
        actualState = DERECHA;
      }
      break;

    case DERECHA:
      if (text == "idle")
      {
        Otto.walk(0, 1000, 1);
        Otto.turn(0, 1000, 1);
        actualState = IDLE;
      }
      if (text == "adelante")
      {
        Otto.turn(0, 1000, 1);
        Otto.walk(4, 2000, 1);
        actualState = ADELANTE;
      }
      if (text == "atras")
      {
        Otto.turn(0, 1000, 1);
        Otto.walk(4, 2000, -1);
        actualState = ATRAS;
      }
      if (text == "izquierda")
      {
        Otto.turn(2, 1000, 1);
        actualState = IZQUIERDA;
      }
      break;
  }
}
