#include "MeBoard101.h"

MeStepper stepper(PORT_1); 

void setup()
{  
  Serial.begin(115200);
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(4000);
  stepper.setAcceleration(4000);
}

void loop()
{
  if(Serial.available())
  {
    char a = Serial.read();
    switch(a)
    {
      case '0':
      stepper.moveTo(0);
      break;
      case '1':
      stepper.moveTo(200);
      break;
      case '2':
      stepper.move(50);
      break;
      case '3':
      stepper.move(100);
      break;
      case '4':
      stepper.move(200);
      break;
      case '5':
      stepper.move(400);
      break;
      case '6':
      stepper.move(600);
      break;
      case '7':
      stepper.move(4000);
      break;
      case '8':
      stepper.move(8000);
      break;
      case '9':
      stepper.move(3200);
      break;
    }
  }
  stepper.run();
}
