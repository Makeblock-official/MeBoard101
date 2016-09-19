#include "MeBoard101.h"

MeEncoderMotor motor(0x09, SLOT2);   //  motor at slot2

void setup()
{
  Serial.begin(115200);
  motor.begin();
}

void loop()
{
  motor.runSpeed(-150);
  delay(3000);
  motor.runSpeed(0);
  delay(2000);
  motor.runSpeed(150);
  delay(3000);
  motor.runSpeed(0);
  delay(2000);
}
