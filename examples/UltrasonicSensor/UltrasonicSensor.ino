#include "MeBoard101.h"
MeUltrasonicSensor sensor(PORT_4);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  Serial.println(sensor.distanceCm());
  delay(100);
}
