#include <MeBoard101.h>
MeShutter s(PORT_8);
void setup() {

}
void loop() {
  s.shotOn();
  delay(500);
  s.shotOff();
  delay(5000);
}