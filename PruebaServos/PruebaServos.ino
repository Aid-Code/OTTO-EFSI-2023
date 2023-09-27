#include <Otto.h>
Otto Otto;  //This is Otto!

#define LeftLeg 9 //Servo pierna izquierda
#define RightLeg 10 //Servo pierna derecha
#define LeftFoot 6 //Servo pie izquierdo
#define RightFoot 3 //Servo pie derecho 
#define Buzzer 13

void setup()
{
  Otto.init(LeftLeg, RightLeg, LeftFoot, RightFoot, true, Buzzer); //Set the servo pins and Buzzer pin
}

void loop()
{
  Otto.turn(2, 1000, 1);
}
