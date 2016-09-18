#include <MeBoard101.h>
MeGasSensor s(PORT_8);
void setup() {
    Serial.begin(115200);
}
void loop() {
  Serial.print(s.readDigital());
  Serial.print(":");
  Serial.println(s.readAnalog());
  delay(200);
}