#include "MeBoard101.h"
MeRGBLed led(PORT_3);
void setup() {
  Serial.begin(115200);
}
int brightness = 0;
int dir = 1;
void loop() {
  brightness+=dir;
  led.setColor(brightness,brightness,brightness);
  led.show();
  if(brightness>50){
    dir = -1;
  }
  if(brightness<1){
    dir = 1;
  }
  delay(20);
}
