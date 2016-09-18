#include "MeBoard101.h"
MeLineFollower sensor(PORT_7);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  Serial.println(sensor.readSensors());
  delay(100);
}
