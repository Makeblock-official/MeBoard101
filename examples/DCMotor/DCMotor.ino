#include "MeBoard101.h"
MeDCMotor dcLeft(PORT_9);
MeDCMotor dcRight(PORT_10);
void setup() {
  Serial.begin(115200);
}

void loop() {
  dcLeft.run(50);
  dcRight.run(-50);
  delay(1000);
  dcLeft.stop();
  dcRight.stop();
  delay(1000);
  dcLeft.run(-50);
  dcRight.run(50);
  delay(1000);
}
