#include "MeBoard101.h"
MeLineFollower linefollower(PORT_3);
MeUltrasonicSensor ultrasonic(PORT_4);
MeDCMotor dcLeft(PORT_2);
MeDCMotor dcRight(PORT_9);
MeInfraredReceiver ir(PORT_8);
#define NORMAL_MODE 1
#define ULTRASONIC_MODE 2
#define LINEFOLLOW_MODE 3
int speed = 100;
int mode = 1;
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
        if(c==0x45){
          mode = NORMAL_MODE;
        }else if(c==0x46){
          mode = ULTRASONIC_MODE;
        }else if(c==0x47){
          mode = LINEFOLLOW_MODE;
        }else if(c==0x40){
          move(speed,speed);
        }else if(c==0x19){
          move(-speed,-speed);
        }else if(c==0x09){
          move(speed,-speed);
        }else if(c==0x07){
          move(-speed,speed);
        }else if(c==0x0C){
          speed = 150;
        }else if(c==0x18){
          speed = 100;
        }else if(c==0x5E){
          speed = 50;
        }
      }
    }
  }else{
    if(mode==NORMAL_MODE){
      move(0,0);
    }
  }
  switch(mode){
    case NORMAL_MODE:
    break;
    case ULTRASONIC_MODE:
      ultrasonicMode();
    break;
    case LINEFOLLOW_MODE:
      lineFollowMode();
    break;
  }
}
void ultrasonicMode(){
  float dist = ultrasonic.distanceCm();
  if(dist<50){
    move(-speed,-speed);
    delay(800);
    if(random(0,100)>50){
      move(speed,-speed);
    }else{
      move(-speed,speed);
    }
    delay(300);
  }else{
    move(speed,speed);
  }
  delay(40);
}
void lineFollowMode(){
  int status = linefollower.readSensors();
  switch(status){
    case 0:
      move(speed,speed);
      break;
    case 1:
      move(speed-80,speed);
      break;
    case 2:
      move(speed,speed-80);
      break;
    case 3:
      break;
  }
  delay(10);
}
void move(int spd1,int spd2){
  if(spd1!=0){
    dcLeft.run(-spd1);
  }else{
    dcLeft.stop();
  }
  if(spd2!=0){
    dcRight.run(spd2);
  }else{
    dcRight.stop();
  }
}
