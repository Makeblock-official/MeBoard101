#include "MeBoard101.h"
MeLightSensor sensor(PORT_8);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  Serial.println(sensor.read());
  delay(100);
}
