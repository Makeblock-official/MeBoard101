# MeBoard101
## How To Use
### Prepare for Arduino
 * Install the Arduino IDE ( https://www.arduino.cc/en/Main/Software )
 * Open Arduino IDE, selecting **Tools menu**, then **Boards** and last **Boards Manager**. 
 ![image](https://raw.githubusercontent.com/Makeblock-official/MeBoard101/master/images/Board_MGR_OK.jpg)
 * Here you can search Intel Curie or 101 to find the core. Click on its box and click on the install button. On the bottom bar of the window you can follow the download and install procedure, including the installation of the proper driver, needed by the operating system to use the 101 board. 
 * From **Tools** select the **Board Arduino/Genuinio 101**
 ![image](https://raw.githubusercontent.com/Makeblock-official/MeBoard101/master/images/101_Board_select.jpg)
 * Download the library for Arduino101/Genuino101 https://github.com/Makeblock-official/MeBoard101/archive/master.zip
 * Copy the makeblock folder to your arduino default library. Your Arduino library folder should now look like this 
   * (on Windows): ```[x:\Users\XXX\Documents]\Arduino\libraries\MeBoard101\src```
   * (on Mac OSX): ```[\Users\XXX\Documents]\Arduino\libraries\MeBoard101\src```
 * Open Arduino IDE, choose the firmware101 from <em>File Examples MeBoard101</em>.
 * Compile and upload firmware according to your Arduino101/Genuino101 board.

 ### Support for Makeblock Electronic Modules
 Module | Arduino IDE | mBlock | Makeblock App
 ------ | ----------- | ------ | ---
 [DC Motor](http://www.makeblock.com/index.php?route=product/search&search=dc%20motor) | ✓ | ✓ | ✓
 [Servo Motor](http://www.makeblock.com/index.php?route=product/search&search=servo) | ✓ | ✓ | ✓ 
 [Encoder Motor](http://www.makeblock.com/index.php?route=product/search&search=Encoder) | ✓ | ✓ | ✘
 [Stepper Motor](http://www.makeblock.com/index.php?route=product/search&search=Stepper) | ✓ | ✓ | ✘  
 [7-Segment Display](http://www.makeblock.com/me-7-segment-serial-display-red) | ✓ | ✓ | ✓
 [RGB LED](http://www.makeblock.com/index.php?route=product/search&search=RGB) | ✓ | ✓ | ✓
 [DSLR Shutter](http://www.makeblock.com/index.php?route=product/search&search=Shutter) | ✓ | ✓ | ✘
 [Ultrasonic Sensor](http://www.makeblock.com/me-ultrasonic-sensor) | ✓ | ✓ | ✓
 [Line Follower](http://www.makeblock.com/me-line-follower) | ✓ | ✓ | ✓
 [Light Sensor](http://www.makeblock.com/me-light-sensor) | ✓ | ✓ | ✓
 [Sound Sensor](http://www.makeblock.com/me-sound-sensor) | ✓ | ✓ | ✓
 [Touch Sensor](http://www.makeblock.com/me-touch-sensor) | ✓ | ✓ | ✓
 [Joystick](http://www.makeblock.com/me-joystick) | ✓ | ✓ | ✓
 [Potentiometer](http://www.makeblock.com/index.php?route=product/search&search=Potentiometer) | ✓ | ✓ | ✓
 [Switch](http://www.makeblock.com/index.php?route=product/search&search=Switch) | ✓ | ✓ | ✓
 [PIRMotion Sensor](http://www.makeblock.com/me-pir-motion-sensor) | ✓ | ✓ | ✓
 [Infrared Receiver](http://www.makeblock.com/me-infrared-receiver-decode) | ✓ | ✓ | ✘
 [Temperature Sensor(DS18B20)](http://www.makeblock.com/me-temperature-sensor-waterproof-ds18b20) | ✓ | ✓ | ✘
 [Temperature and Humidity Sensor](http://www.makeblock.com/me-temperature-and-humidity-sensor) | ✓ | ✓ | ✓
 [Flame Sensor](http://www.makeblock.com/me-flame-sensor) | ✓ | ✓ | ✓
 [Gas Sensor](http://www.makeblock.com/me-gas-sensor) | ✓ | ✓ | ✓
 [Button](http://www.makeblock.com/me-4-button) | ✘ | ✘ | ✘
 [Gyro on Board](https://www.arduino.cc/en/Tutorial/Genuino101CurieIMURawImuDataSerial) | ✓ | ✓ | ✘