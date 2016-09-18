#include "MeBoard101.h"
MeTouchSensor sensor(PORT_3);
void setup() {
  Serial.begin(115200);
  sensor.setTogMode(1);
}

void loop() {
  Serial.println(sensor.touched());
  delay(100);
}
