#include "MeBoard101.h"
MeSoundSensor sensor(PORT_8);
void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println(sensor.read());
  delay(100);
}
