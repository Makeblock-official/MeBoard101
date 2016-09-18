#include "MeBoard101.h"
MeInfraredReceiver ir(PORT_8);
void setup() {
  Serial.begin(115200);
  ir.begin();
}
void loop() {
  if(ir.buttonState()){
    if(ir.available()){
      int c = ir.read();
      if(c>0&&c<255){
        Serial.print("IR Code:");
        Serial.println(c);
      }
    }
  }
}
