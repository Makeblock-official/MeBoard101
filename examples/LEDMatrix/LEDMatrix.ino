#include "MeBoard101.h"
MeLEDMatrix ledMx(PORT_3);
void setup() {
  Serial.begin(115200);
  ledMx.setBrightness(3);
  ledMx.setColorIndex(1);
}

void loop() {
  ledMx.showClock(12,03,1);
  delay(2000);
  ledMx.drawStr(0,7,"hello");
  delay(2000);
  ledMx.showNum(1.23);
  delay(2000);
}
