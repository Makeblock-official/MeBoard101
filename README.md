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

 Module | Arduino IDE | mBlock | App
 ------ | ----------- | ------ | ---
 DC Motor | y | y | y