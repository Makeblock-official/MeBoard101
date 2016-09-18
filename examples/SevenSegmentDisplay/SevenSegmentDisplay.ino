#include <MeBoard101.h>
Me7SegmentDisplay seg(PORT_8);
void setup() {

}
float t = 0;
void loop() {
  t+=0.1;
  if(t>60){
    t = 0;
  }
  seg.display(t);
  delay(100);
}