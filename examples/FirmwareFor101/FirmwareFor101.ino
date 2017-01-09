#include <CurieBLE.h>
#include <CurieIMU.h>
#include <MeBoard101.h>
#include <MadgwickAHRS.h>

BLEPeripheral blePeripheral;         // BLE Peripheral Device (the board you're programming)
BLEService firmwareService("FFE1");  // BLE Service

BLECharacteristic readCharacteristic("FFE2", BLENotify,20);
BLECharacteristic writeCharacteristic("FFE3", BLEWriteWithoutResponse,20);

MeDCMotor dc;
Servo servos[8];
MeEncoderMotor encoders[2];
MeStepper steppers[4];
MeRGBLed led;
MeUltrasonicSensor us(PORT_4);
Me7SegmentDisplay seg;
MePort generalDevice;
MeJoystick joystick;
MeHumiture humiture;
MeFlameSensor FlameSensor;
MeGasSensor GasSensor;
MeTouchSensor touchSensor;
MeLEDMatrix ledMx;
Me4Button buttonSensor;
MeLineFollower line(PORT_3);
MeInfraredReceiver *ir = NULL;  //PORT_8
Madgwick filter;

typedef struct MeModule
{
    int device;
    int port;
    int slot;
    int pin;
    int index;
    float values[3];
} MeModule;

union{
    byte byteVal[4];
    float floatVal;
    long longVal;
}val;

union{
  byte byteVal[8];
  double doubleVal;
}valDouble;

union{
  byte byteVal[2];
  short shortVal;
}valShort;

MeModule modules[12];

#define BLUE_TOOTH        0
#define IR_CONTROLER      1


#define IR_CONTROLER_MODE   0
#define ULTRASONIC_MODE     1
#define LINEFOLLOW_MODE     2
#define IR_CUSTOM_MODE      4


int analogs[6]={A0,A1,A2,A3,A4,A5};

  
String mVersion = "11.01.001";
boolean isAvailable = false;
boolean isBluetooth = false;

int len = 52;
unsigned char buffer[52];
unsigned char bleBuffer[52];
byte bufferIndex = 0;
byte bleBufferIndex = 0;
byte dataLen;
byte modulesLen=0;
boolean isStart = false;
unsigned char irRead;
char serialRead;

int arduino101_mode = IR_CONTROLER_MODE;
int LineFollowFlag=0;
int moveSpeed = 190;
int turnSpeed = 200;
int minSpeed = 45;
uint8_t controlflag = IR_CONTROLER;
int distance=0;
int randnum = 0;
int factor = 23;
unsigned long microsPrevious;
float accelScale, gyroScale;
float roll, pitch, yaw;

boolean leftflag = false;
boolean rightflag = false;
bool isNotify = false;

#define VERSION                0
#define ULTRASONIC_SENSOR      1
#define TEMPERATURE_SENSOR     2
#define LIGHT_SENSOR           3
#define POTENTIONMETER         4
#define JOYSTICK               5
#define GYRO                   6
#define SOUND_SENSOR           7
#define RGBLED                 8
#define SEVSEG                 9
#define MOTOR                  10
#define SERVO                  11
#define ENCODER                12
#define IR                     13
#define IRREMOTE               14
#define PIRMOTION              15
#define INFRARED               16
#define LINEFOLLOWER           17
#define IRREMOTECODE           18
#define SHUTTER                20
#define LIMITSWITCH            21
#define BUTTON                 22
#define HUMITURE               23
#define FLAMESENSOR            24
#define GASSENSOR              25
#define COMPASS                26
#define TEMPERATURE_SENSOR_1   27
#define DIGITAL                30
#define ANALOG                 31
#define PWM                    32
#define SERVO_PIN              33
#define TONE                   34
#define BUTTON_INNER           35
#define ULTRASONIC_ARDUINO     36
#define PULSEIN                37
#define STEPPER                40
#define LEDMATRIX              41
#define TIMER                  50
#define TOUCH_SENSOR           51
#define JOYSTICK_MOVE          52
#define COMMON_COMMONCMD       60
  //Secondary command
  #define SET_STARTER_MODE     0x10
  #define SET_AURIGA_MODE      0x11
  #define SET_MEGAPI_MODE      0x12
  #define GET_BATTERY_POWER    0x70
  #define GET_AURIGA_MODE      0x71
  #define GET_MEGAPI_MODE      0x72
#define ENCODER_BOARD          61
  //Read type
  #define ENCODER_BOARD_POS    0x01
  #define ENCODER_BOARD_SPEED  0x02

#define ENCODER_PID_MOTION     62
  //Secondary command
  #define ENCODER_BOARD_POS_MOTION         0x01
  #define ENCODER_BOARD_SPEED_MOTION       0x02
  #define ENCODER_BOARD_PWM_MOTION         0x03
  #define ENCODER_BOARD_SET_CUR_POS_ZERO   0x04
  #define ENCODER_BOARD_CAR_POS_MOTION     0x05
  
#define PM25SENSOR             63
  //Secondary command
  #define GET_PM1_0         0x01
  #define GET_PM2_5         0x02
  #define GET_PM10          0x03

#define GET 1
#define RUN 2
#define RESET 4
#define START 5
float angleServo = 90.0;
int servo_pins[8]={0,0,0,0,0,0,0,0};
unsigned char prevc=0;
double lastTime = 0.0;
double currentTime = 0.0;
uint8_t keyPressed = 0;
uint8_t command_index = 0;
uint8_t receivedBuffer[64];
int receivedBufferLength = 0;
bool isReceiving = false;

void Forward()
{
  dc.reset(PORT_2);
  dc.run(-moveSpeed);
  dc.reset(PORT_9);
  dc.run(moveSpeed);
}

void Backward()
{
  dc.reset(PORT_2);
  dc.run(moveSpeed);
  dc.reset(PORT_9);
  dc.run(-moveSpeed);
}

void BackwardAndTurnLeft()
{
  dc.reset(PORT_2);
  dc.run(moveSpeed/2);
  dc.reset(PORT_9);
  dc.run(-moveSpeed);
}

void BackwardAndTurnRight()
{
  dc.reset(PORT_2);
  dc.run(moveSpeed);
  dc.reset(PORT_9);
  dc.run(-moveSpeed/2);
}

void TurnLeft()
{
  dc.reset(PORT_2);
  dc.run(moveSpeed);
  dc.reset(PORT_9);
  dc.run(moveSpeed);
}

void TurnRight()
{
  dc.reset(PORT_2);
  dc.run(-moveSpeed);
  dc.reset(PORT_9);
  dc.run(-moveSpeed);
}

void Stop()
{
  dc.reset(PORT_2);
  dc.run(0);
  dc.reset(PORT_9);
  dc.run(0);
}
void ChangeSpeed(int spd)
{
  moveSpeed = spd;
}

float convertRawAcceleration(int aRaw) {
  float a = (aRaw * 2.0)*90.0 / 32768.0;
  return a;
}

float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}

void gyroLoop(void)
{
  int aix, aiy, aiz;
  int gix, giy, giz;
  float ax, ay, az;
  float gx, gy, gz;
  unsigned long millisNow;
  millisNow = millis();
  if(millisNow - microsPrevious >= 40)
  {
    // read raw data from CurieIMU
    CurieIMU.readMotionSensor(aix, aiy, aiz, gix, giy, giz);

    // convert from raw data to gravity and degrees/second units
    ax = convertRawAcceleration(aix);
    ay = convertRawAcceleration(aiy);
    az = convertRawAcceleration(aiz);
    gx = convertRawGyro(gix);
    gy = convertRawGyro(giy);
    gz = convertRawGyro(giz);

    // update the filter, which computes orientation
    filter.updateIMU(gx, gy, gz, ax, ay, az);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    yaw = filter.getYaw();
    microsPrevious = microsPrevious + 40; 
  }
}
double getAngle(uint8_t index)
{
  if(index == 1)
  {
    return roll;
  }
  else if(index == 2)
  {
    return pitch;
  }
  else if(index == 3)
  {
    return yaw;
  }
} 

void parseBleData(unsigned char c){
  if(c==0x55&&isStart==false){
   if(prevc==0xff){
    bufferIndex =1;
    isStart = true;
   }
  }else{
    prevc = c;
    if(isStart){
      if(bufferIndex==2){
       dataLen = c; 
      }else if(bufferIndex>2){
        dataLen--;
      }
      writeBuffer(bufferIndex,c);
    }
  }
   bufferIndex++;
   if(bufferIndex>51){
    bufferIndex=0; 
    isStart=false;
   }
   if(isStart&&dataLen==0&&bufferIndex>3){ 
      isStart = false;
      parseData(); 
      bufferIndex=0;
   }
}
unsigned char readBuffer(int index){
 return buffer[index]; 
}
void writeBuffer(int index,unsigned char c){
  buffer[index]=c; 
}
void writeBleBuffer(int index,unsigned char c){
  bleBuffer[index]=c; 
}
void writeBleHead(){
  bleBufferIndex = 0;
  appendBle(0xff);
  appendBle(0x55);
}
void flushBle(){
  appendBle('\r');
  appendBle('\n');
  isNotify = true;
}
void appendBle(unsigned char c){
  bleBuffer[bleBufferIndex] = c;
  bleBufferIndex++;
  Serial.write(c);
  Serial1.write(c);
}
void mBotCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  // central wrote new value to characteristic, update LED
  const unsigned char *c = writeCharacteristic.value();
  int len = writeCharacteristic.valueLength();
  isReceiving = true;
  for(int i=0;i<len;i++){
    receivedBuffer[receivedBufferLength++] = (c[i]);
  }
  isReceiving = false;
}
void parseData(){
  
  isStart = false;
  int idx = readBuffer(3);
  command_index = (uint8_t)idx;
  int action = readBuffer(4);
  int device = readBuffer(5);
  switch(action){
    case GET:{
        writeBleHead();
        appendBle(idx);
        readSensor(device);
        flushBle();
     }
     break;
     case RUN:{
       runModule(device);
       callOK();
     }
      break;
      case RESET:{
        //reset

        dc.reset(PORT_1);
        dc.run(0);
        dc.reset(PORT_2);
        dc.run(0);
        dc.reset(PORT_9);
        dc.run(0);
        dc.reset(PORT_10);
        dc.run(0);
        encoders[0].runSpeed(0);
        encoders[1].runSpeed(0);
        callOK();
      }
     break;
     case START:{
        //start
        callOK();
      }
     break;
  }
}
void callOK(){
    appendBle(0xff);
    appendBle(0x55);
    flushBle();
}
void appendByte(char c){
  appendBle(1);
  appendBle(c);
}
void appendString(String s){
  int len = s.length();
  appendBle(4);
  appendBle(len);
  for(int i=0;i<len;i++){
    appendBle(s.charAt(i));
  }
}
void appendFloat(float value){ 
     appendBle(0x2);
     val.floatVal = value;
     appendBle(val.byteVal[0]);
     appendBle(val.byteVal[1]);
     appendBle(val.byteVal[2]);
     appendBle(val.byteVal[3]);
}
void appendShort(double value){
     appendBle(3);
     valShort.shortVal = value;
     appendBle(valShort.byteVal[0]);
     appendBle(valShort.byteVal[1]);
}
void appendDouble(double value){
     appendBle(2);
     valDouble.doubleVal = value;
     appendBle(valDouble.byteVal[0]);
     appendBle(valDouble.byteVal[1]);
     appendBle(valDouble.byteVal[2]);
     appendBle(valDouble.byteVal[3]);
}
short readShort(int idx){
  valShort.byteVal[0] = readBuffer(idx);
  valShort.byteVal[1] = readBuffer(idx+1);
  return valShort.shortVal; 
}
float readFloat(int idx){
  val.byteVal[0] = readBuffer(idx);
  val.byteVal[1] = readBuffer(idx+1);
  val.byteVal[2] = readBuffer(idx+2);
  val.byteVal[3] = readBuffer(idx+3);
  return val.floatVal;
}
long readLong(int idx){
  val.byteVal[0] = readBuffer(idx);
  val.byteVal[1] = readBuffer(idx+1);
  val.byteVal[2] = readBuffer(idx+2);
  val.byteVal[3] = readBuffer(idx+3);
  return val.longVal;
}
char _receiveStr[20] = {};
uint8_t _receiveUint8[16] = {};
char* readString(int idx,int len){
  for(int i=0;i<len;i++){
    _receiveStr[i]=readBuffer(idx+i);
  }
  _receiveStr[len] = '\0';
  return _receiveStr;
}
uint8_t* readUint8(int idx,int len){
  for(int i=0;i<len;i++){
    if(i>15){
      break;
    }
    _receiveUint8[i] = readBuffer(idx+i);
  }
  return _receiveUint8;
}
void runModule(int device){
  //0xff 0x55 0x6 0x0 0x1 0xa 0x9 0x0 0x0 0xa
  int port = readBuffer(6);
  int pin = port;
  switch(device){
   case MOTOR:{
     int speed = readShort(7);
     controlflag = BLUE_TOOTH;
     dc.reset(port);
     dc.run(speed);
    }
    break;
    case JOYSTICK:{
     int leftSpeed = readShort(6);
     int rightSpeed = readShort(8);
     controlflag = BLUE_TOOTH;
     dc.reset(PORT_2);
     dc.reset(PORT_9);
     dc.run(leftSpeed);
     dc.run(rightSpeed);
    }
    break;
    case STEPPER:{
     int maxSpeed = readShort(7);
     long distance = readLong(9);
     if(port==PORT_1){
      steppers[0] = MeStepper(PORT_1);
      steppers[0].moveTo(distance);
      steppers[0].setMaxSpeed(maxSpeed);
      steppers[0].setSpeed(maxSpeed);
     }else if(port==PORT_2){
      steppers[1] = MeStepper(PORT_2);
      steppers[1].moveTo(distance);
      steppers[1].setMaxSpeed(maxSpeed);
      steppers[1].setSpeed(maxSpeed);
     }else if(port==PORT_9){
      steppers[2] = MeStepper(PORT_9);
      steppers[2].moveTo(distance);
      steppers[2].setMaxSpeed(maxSpeed);
      steppers[2].setSpeed(maxSpeed);
     }else if(port==PORT_10){
      steppers[3] = MeStepper(PORT_10);
      steppers[3].moveTo(distance);
      steppers[3].setMaxSpeed(maxSpeed);
      steppers[3].setSpeed(maxSpeed);
     }
    }
    break;
    case ENCODER:{
      int slot = readBuffer(7);
      int maxSpeed = readShort(8);
      float distance = readFloat(10);
      if(slot==SLOT_1){
         encoders[0].move(distance,maxSpeed);
      }else if(slot==SLOT_2){
         encoders[1].move(distance,maxSpeed);
      }
    }
    break;
   case RGBLED:{
     int slot = readBuffer(7);
     int idx = readBuffer(8);
     int r = readBuffer(9);
     int g = readBuffer(10);
     int b = readBuffer(11);
     led.reset(port,slot);
     if(idx>0)
     {
       led.setColorAt(idx-1,r,g,b); 
     }
     else
     {
       led.setColor(r,g,b); 
     }
     led.show();
   }
   break;
   case LEDMATRIX:{
     if(ledMx.getPort()!=port){
       ledMx.reset(port);
     }
     int action = readBuffer(7);
     if(action==1){
            int px = buffer[8];
            int py = buffer[9];
            int len = readBuffer(10);
            char *s = readString(11,len);
            ledMx.drawStr(px,py,s);
      }else if(action==2){
            int px = readBuffer(8);
            int py = readBuffer(9);
            uint8_t *ss = readUint8(10,16);
            ledMx.drawBitmap(px,py,16,ss);
      }else if(action==3){
            int point = readBuffer(8);
            int hours = readBuffer(9);
            int minutes = readBuffer(10);
            ledMx.showClock(hours,minutes,point);
     }else if(action == 4){
            ledMx.showNum(readFloat(8),3);
     }
   }
   break;
   case SERVO:{
     int slot = readBuffer(7);
     pin = slot==1?mePort[port].s1:mePort[port].s2;
     int v = readBuffer(8);
     Servo sv = servos[searchServoPin(pin)];
     if(v >= 0 && v <= 180)
     {
       if(!sv.attached())
       {
         sv.attach(pin);
       }
       sv.write(v);
     }
   }
   break;
   case SEVSEG:{
     float v = readFloat(7);
     if(seg.getPort()!=port){
       seg.reset(port);
     }
     seg.display(v);
   }
   break;
   case LIGHT_SENSOR:{
     int v = readBuffer(7);
     if(generalDevice.getPort()!=port){
       generalDevice.reset(port);
     }
     generalDevice.dWrite1(v);
   }
   break;
   case SHUTTER:{
     int v = readBuffer(7);
     if(generalDevice.getPort()!=port){
       generalDevice.reset(port);
     }
     if(v<2){
       generalDevice.dWrite1(v);
     }else{
       generalDevice.dWrite2(v-2);
     }
   }
   break;
   case DIGITAL:{
     int v = readBuffer(7);
     pinMode(pin,OUTPUT);
     digitalWrite(pin,v);
   }
   break;
   case PWM:{
     int v = readBuffer(7);
     pinMode(pin,OUTPUT);
     analogWrite(pin,v);
   }
   break;
   case TONE:{
     pinMode(pin,OUTPUT);
     int hz = readShort(7);
     int ms = readShort(9);
     if(ms>0){
       tone(pin, hz, ms); 
     }else{
       noTone(pin); 
     }
   }
   break;
   case SERVO_PIN:{
     int v = readBuffer(7);
     Servo sv = servos[searchServoPin(pin)]; 
     if(v >= 0 && v <= 180)
     {
       if(!sv.attached())
       {
         sv.attach(pin);
       }
       sv.write(v);
     }
   }
   break;
   case TIMER:{
    lastTime = millis()/1000.0; 
   }
   break;
   default:
    callOK();
   break;
  }
}

int searchServoPin(int pin){
    for(int i=0;i<8;i++){
      if(servo_pins[i] == pin){
        return i;
      }
      if(servo_pins[i]==0){
        servo_pins[i] = pin;
        return i;
      }
    }
    return 0;
}

void readSensor(int device){
  /**************************************************
      ff 55 len idx action device port slot data a
      0  1  2   3   4      5      6    7    8
  ***************************************************/
  float value=0.0;
  int port,slot,pin;
  port = readBuffer(6);
  pin = port;
  switch(device){
   case  ULTRASONIC_SENSOR:{
     if(us.getPort()!=port){
       us.reset(port);
     }
     value = us.distanceCm();
//     writeBleHead();
//     appendBle(command_index);
     appendFloat(value);
   }
   break;
   case  TEMPERATURE_SENSOR:{
//     slot = readBuffer(7);
//     if(ts.getPort()!=port||ts.getSlot()!=slot){
//       ts.reset(port,slot);
//     }
//     value = ts.temperature();
//     appendFloat(value);
   }
   break;
   case  LIGHT_SENSOR:
   case  SOUND_SENSOR:
   case  POTENTIONMETER:{
     if(generalDevice.getPort()!=port){
       generalDevice.reset(port);
       pinMode(generalDevice.pin2(),INPUT);
     }
     value = generalDevice.aRead2();
     appendFloat(value);
   }
   break;
   case  JOYSTICK:{
     slot = readBuffer(7);
     if(joystick.getPort() != port){
       joystick.reset(port);
     }
     value = joystick.read(slot);
     appendFloat(value);
   }
   break;
    case  INFRARED:
      {
        arduino101_mode = IR_CUSTOM_MODE;
        if(ir == NULL)
        {
          ir = new MeInfraredReceiver(port);
          ir->begin();
        }
        else if(ir->getPort() != port)
        {
          delete ir;
          ir = new MeInfraredReceiver(port);
          ir->begin();
        }
        irRead = ir->getCode();
        if((irRead < 255) && (irRead > 0))
        {
          appendFloat((float)irRead);
        }
        else
        {
          appendFloat(0);
        }
      }
      break;
   case  PIRMOTION:{
     if(generalDevice.getPort()!=port){
       generalDevice.reset(port);
       pinMode(generalDevice.pin2(),INPUT);
     }
     value = generalDevice.dRead2();
     appendFloat(value);
   }
   break;
   case  LINEFOLLOWER:{
     if(generalDevice.getPort()!=port){
       generalDevice.reset(port);
         pinMode(generalDevice.pin1(),INPUT);
         pinMode(generalDevice.pin2(),INPUT);
     }
     value = generalDevice.dRead1()*2+generalDevice.dRead2();
     appendFloat(value);
   }
   break;
   case LIMITSWITCH:{
     slot = readBuffer(7);
     if(generalDevice.getPort()!=port||generalDevice.getSlot()!=slot){
       generalDevice.reset(port,slot);
     }
     if(slot==1){
       pinMode(generalDevice.pin1(),INPUT_PULLUP);
       value = !generalDevice.dRead1();
     }else{
       pinMode(generalDevice.pin2(),INPUT_PULLUP);
       value = !generalDevice.dRead2();
     }
     appendFloat(value);  
   }
   break;
   case HUMITURE:{
     uint8_t index = readBuffer(7);
     if(humiture.getPort()!=port){
       humiture.reset(port);
     }
     uint8_t HumitureData;
     humiture.update();
     HumitureData = humiture.getValue(index);
     appendByte(HumitureData);
   }
   break;
   case FLAMESENSOR:{
     if(FlameSensor.getPort()!=port){
       FlameSensor.reset(port);
       FlameSensor.setpin(FlameSensor.pin2(),FlameSensor.pin1());
     }
     int16_t FlameData; 
     FlameData = FlameSensor.readAnalog();
     appendShort(FlameData);
   }
   break;
   case GASSENSOR:{
     if(GasSensor.getPort()!=port){
       GasSensor.reset(port);
       GasSensor.setpin(GasSensor.pin2(),GasSensor.pin1());
     }
     int16_t GasData; 
     GasData = GasSensor.readAnalog();
     appendShort(GasData);
   }
   break;
   case  GYRO:{
       int axis = readBuffer(7);
       value = getAngle(axis);
       appendFloat(value);
   }
   break;
   case  VERSION:{
     appendString(mVersion);
   }
   break;
   case  DIGITAL:{
     pinMode(pin,INPUT);
     appendFloat(digitalRead(pin));
   }
   break;
   case  ANALOG:{
     pin = analogs[pin];
     pinMode(pin,INPUT);
     appendFloat(analogRead(pin));
   }
   break;
   case  PULSEIN:{
     int pw = readShort(7);
     pinMode(pin, INPUT);
     appendShort(pulseIn(pin,HIGH,pw));
   }
   break;
   case ULTRASONIC_ARDUINO:{
     int trig = readBuffer(6);
     int echo = readBuffer(7);
     pinMode(trig,OUTPUT);
     digitalWrite(trig,LOW);
     delayMicroseconds(2);
     digitalWrite(trig,HIGH);
     delayMicroseconds(10);
     digitalWrite(trig,LOW);
     pinMode(echo, INPUT);
     appendFloat(pulseIn(echo,HIGH,30000)/58.0);
   }
   break;
   case TIMER:{
     appendFloat((float)currentTime);
   }
   break;
   case BUTTON:
   {
     if(buttonSensor.getPort() != port){
       buttonSensor.reset(port);
     }
     appendByte(keyPressed==readBuffer(7));
   }
   break;
   case TOUCH_SENSOR:
   {
     if(touchSensor.getPort() != port){
       touchSensor.reset(port);
     }
     appendByte(touchSensor.touched());
   }
   break;
  }
}

void lineFollow(void)
{
  uint8_t val;
  val = line.readSensors();
  if(moveSpeed >100)
  {
    moveSpeed=100;
  }
  switch (val)
  {
    case S1_IN_S2_IN:
      Forward();
      LineFollowFlag=10;
      break;

    case S1_IN_S2_OUT:
       Forward();
      if (LineFollowFlag>1) LineFollowFlag--;
      break;

    case S1_OUT_S2_IN:
      Forward();
      if (LineFollowFlag<20) LineFollowFlag++;
      break;

    case S1_OUT_S2_OUT:
      if(LineFollowFlag==10) Backward();
      if(LineFollowFlag<10) TurnLeft();
      if(LineFollowFlag>10) TurnRight();
      break;
  }
//  delay(50);
}

void ultrCarProcess(void)
{
  distance = us.distanceCm();
  randomSeed(analogRead(A4));
  if(moveSpeed >100)
  {
    moveSpeed=100;
  }
  if((distance > 10) && (distance < 40))
  {
    randnum=random(300);
    if((randnum > 190) && (!rightflag))
    {
      leftflag=true;
      TurnLeft();   
    }
    else
    {
      rightflag=true;
      TurnRight();  
    }
  }
  else if(distance < 10)
  {
    randnum=random(300);
    if(randnum > 190)
    {
      BackwardAndTurnLeft();
    }
    else
    {
      BackwardAndTurnRight();
    }
  }
  else
  {
    leftflag=false;
    rightflag=false;
    Forward();
  }
}

void IrProcess(void)
{
  ir->loop();
  irRead = ir->getCode();
  if((arduino101_mode != IR_CONTROLER_MODE) &&
     (irRead != IR_BUTTON_TEST) && 
     (irRead != IR_BUTTON_A) &&
     (irRead != IR_BUTTON_B) &&
     (irRead != IR_BUTTON_C))
  {
    return;
  }
  switch(irRead)
  {
    case IR_BUTTON_A:
      Stop();
      arduino101_mode = IR_CONTROLER_MODE;
      moveSpeed=100;
      break;
    case IR_BUTTON_B:
      Stop();
      arduino101_mode = ULTRASONIC_MODE;
      moveSpeed=100;
      break;
    case IR_BUTTON_C:
      Stop();
      arduino101_mode = LINEFOLLOW_MODE;
      moveSpeed=100;
      break;
    case IR_BUTTON_PLUS:
      controlflag = IR_CONTROLER;
      Forward();
      break;
    case IR_BUTTON_MINUS:
      controlflag = IR_CONTROLER;
      Backward();
      break;
    case IR_BUTTON_NEXT:
      controlflag = IR_CONTROLER;
      TurnRight();
      break;
    case IR_BUTTON_PREVIOUS:
      controlflag = IR_CONTROLER;
      TurnLeft();
      break;
    case IR_BUTTON_9:
      controlflag = IR_CONTROLER;
      ChangeSpeed(factor*9+minSpeed);
      break;
    case IR_BUTTON_8:
      controlflag = IR_CONTROLER;
      ChangeSpeed(factor*8+minSpeed);
      break;
    case IR_BUTTON_7:
      controlflag = IR_CONTROLER;
      ChangeSpeed(factor*7+minSpeed);
      break;
    case IR_BUTTON_6:
      controlflag = IR_CONTROLER;
      ChangeSpeed(factor*6+minSpeed);
      break;
    case IR_BUTTON_5:
      controlflag = IR_CONTROLER;
      ChangeSpeed(factor*5+minSpeed);
      break;
    case IR_BUTTON_4:
      controlflag = IR_CONTROLER;
      ChangeSpeed(factor*4+minSpeed);
      break;
    case IR_BUTTON_3:
      controlflag = IR_CONTROLER;
      ChangeSpeed(factor*3+minSpeed);
      break;
    case IR_BUTTON_2:
      controlflag = IR_CONTROLER;
      ChangeSpeed(factor*2+minSpeed);
      break;
    case IR_BUTTON_1:
      controlflag = IR_CONTROLER;
      ChangeSpeed(factor*1+minSpeed);
      break;
    case IR_BUTTON_TEST:
      Stop();
      while(ir->buttonState() != 0)
      {
        ir->loop();
      }
      arduino101_mode = arduino101_mode + 1;
      if(arduino101_mode == 3)
      { 
        arduino101_mode = 0;
      }
      break;
    default:
      if(controlflag == IR_CONTROLER)
      {
        Stop();
      }
      break;
  }
}

void setup(){
  Serial.begin(115200);
  Serial1.begin(115200);
  blePeripheral.setLocalName("Makeblock");
  blePeripheral.setDeviceName("Makeblock");

  blePeripheral.setAdvertisedServiceUuid(firmwareService.uuid());

  // add service and characteristic:
  blePeripheral.addAttribute(firmwareService);
  blePeripheral.addAttribute(writeCharacteristic);
  blePeripheral.addAttribute(readCharacteristic);

  writeCharacteristic.setEventHandler(BLEWritten, mBotCharacteristicWritten);
// set an initial value for the characteristic
  // begin advertising BLE service:
  blePeripheral.begin();
  CurieIMU.begin();
  CurieIMU.setGyroRate(25);
  CurieIMU.setAccelerometerRate(25);
  filter.begin(25);
  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);
  // Set the gyroscope range to 250 degrees/second
  CurieIMU.setGyroRange(250);

// initialize variables to pace updates to correct rate
  microsPrevious = millis();;

  ledMx.setBrightness(6);
  ledMx.setColorIndex(1);
  encoders[0] = MeEncoderMotor(SLOT_1);
  encoders[1] = MeEncoderMotor(SLOT_2);
  encoders[0].begin();
  encoders[1].begin();
  delay(500);
  encoders[0].runSpeed(0);
  encoders[1].runSpeed(0);

  if(ir == NULL)
  {
    ir = new MeInfraredReceiver(PORT_8);
    ir->begin();
  }
}

void loop(){
  keyPressed = buttonSensor.pressed();
  blePeripheral.poll();
  BLECentral central = blePeripheral.central();
  if (central) {
    if(readCharacteristic.subscribed()) {
      if(isNotify){
        readCharacteristic.setValue(bleBuffer,bleBufferIndex);
        bleBufferIndex = 0;
        isNotify = false;
      }
    }
  }
  if(isReceiving==false){
    for(int i=0;i<receivedBufferLength;i++){
      parseBleData(receivedBuffer[i]);
    }
    receivedBufferLength = 0;
  }
  if(Serial.available()){
    parseBleData(Serial.read());
  }
  if(Serial1.available()){
    parseBleData(Serial1.read());
  }
  steppers[0].runSpeedToPosition();
  steppers[1].runSpeedToPosition();
  steppers[2].runSpeedToPosition();
  steppers[3].runSpeedToPosition();
  gyroLoop();
  if(arduino101_mode != IR_CUSTOM_MODE) 
  {
    IrProcess();
  }
  else
  {
    ir->loop();
  }
  if(arduino101_mode == ULTRASONIC_MODE)
  {
    ultrCarProcess();    
  }
  else if(arduino101_mode == LINEFOLLOW_MODE) 
  {
    lineFollow();
  }
}

