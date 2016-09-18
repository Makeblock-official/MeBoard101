#include "MeBoard101.h"
MeHumiture sensor(PORT_3);
void setup() {
  Serial.begin(115200);
}

void loop() {
  sensor.update();
  Serial.print("Humidity:");
  Serial.print(sensor.getHumidity());
  Serial.print("% Temperature:");
  Serial.print(sensor.getTemperature());
  Serial.println(" Celsius");
  delay(100);
}
