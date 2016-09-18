#include <MeBoard101.h>
Me4Button s(PORT_8);
void setup() {
    Serial.begin(115200);
}
void loop() {
  Serial.println(s.pressed());
  delay(200);
}