#include "MeBoard101.h"
MeJoystick joystick(PORT_8);
void setup() {
  Serial.begin(115200);
}
void loop() {
  Serial.print(joystick.readX());
  Serial.print(":");
  Serial.println(joystick.readY());
  delay(20);
}
