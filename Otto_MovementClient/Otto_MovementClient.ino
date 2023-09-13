#include <Otto.h>
#include <SPI.h>
#include <RF24.h>

Otto Otto;

//States Defines
#define IDLE 0
#define ADELANTE 1
#define ATRAS 2
#define IZQUIERDA 3
#define DERECHA 4

//Componentes
#define LeftLeg 2 //Servo pierna izquierda
#define RightLeg 3 //Servo pierna derecha
#define LeftFoot 4 //Servo pie izquierdo
#define RightFoot 5 //Servo pie derecho 
#define Buzzer  13

RF24 radio(7, 8); // CE, CSN

const byte identificacion[6] = "00001";
int actualState;

void setup()
{
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

void loop()
{
  if (radio.available())
  {
    char texto[32] = "";
    radio.read(&texto, sizeof(texto));
    Serial.println(texto);

    Movement(texto); //Le envia la variable texto recibida del server 
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
