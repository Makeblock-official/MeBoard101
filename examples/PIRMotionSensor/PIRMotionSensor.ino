#include "MeBoard101.h"
MePIRMotionSensor sensor(PORT_3);
void setup() {
  Serial.begin(115200);
  sensor.setPirMotionMode(1);
}

void loop() {
  Serial.println(sensor.isHumanDetected());
  delay(100);
}
