#include <Servo.h>
#include <SPI.h>
#include <RF24.h>

#define VRX A5    // Arduino pin connected to VRX pin
#define VRY A4  // Arduino pin connected to Servo motor 1

int i = 2000;
int x;
int y;

RF24 radio(7, 8); // CE, CSN

const byte identificacion[6] = "00001";

void setup() {
  Serial.begin(9600);

  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);

  radio.begin();
  radio.openWritingPipe(identificacion);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

}

void loop() {
  
  x = analogRead(VRX);
  y = analogRead(VRY);

  Serial.println(x);

  radio.write(&i, sizeof(i));
  radio.write(&x, sizeof(x));
  radio.write(&y, sizeof(y));

}
