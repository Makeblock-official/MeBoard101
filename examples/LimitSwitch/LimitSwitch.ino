#include <MeBoard101.h>
MeLimitSwitch s(PORT_8,SLOT_1);
void setup() {
    Serial.begin(115200);
}
void loop() {
  Serial.println(s.touched());
  delay(200);
}