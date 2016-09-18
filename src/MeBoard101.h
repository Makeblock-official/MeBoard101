/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \brief   Driver for MeOrion board.
 * @file    MeOrion.h
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/09/02
 * @brief   Driver for MeOrion board.
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2016, MakeBlock. Use is subject to license \n
 * conditions. The main licensing options available are GPL V2 or Commercial: \n
 *
 * \par Open Source Licensing GPL V2
 * This is the appropriate option if you want to share the source code of your \n
 * application with everyone you distribute it to, and you also want to give them \n
 * the right to share who uses it. If you wish to use this software under Open \n
 * Source Licensing, you must contribute all your source code to the open source \n
 * community in accordance with the GPL Version 2 when your application is \n
 * distributed. See http://www.gnu.org/copyleft/gpl.html
 *
 * \par Description
 * This file is the driver for MeOrion hoard by MakeBlock.
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/09/01     1.0.0            Rebuild the old lib.
 * Rafael Lee       2015/09/02     1.0.1            Added some comments and macros.
 * </pre>
 */
#ifndef MeBoard101_H
#define MeBoard101_H
#define ARM_BOARD
#include <Arduino.h>
#include "MeConfig.h"

// Supported Modules drive needs to be added here
#include "Me7SegmentDisplay.h"
#include "MeUltrasonicSensor.h"
#include "MeLineFollower.h"
#include "MeDCMotor.h"
#include "MeLightSensor.h"
#include "MePotentiometer.h"
#include "MeSoundSensor.h"
#include "MePIRMotionSensor.h"
#include "MeHumitureSensor.h"
#include "MeTouchSensor.h"
#include "MeLEDMatrix.h"
#include "MeRGBLed.h"
#include "Adafruit_NeoPixel.h"
#include "MeJoystick.h"
#include "Me4Button.h"
#include "MeTemperature.h"
#include "MeFlameSensor.h"
#include "MeGasSensor.h"
#include "MeInfraredReceiver.h"
/*


#include "MeShutter.h"


#include "MeLimitSwitch.h"
#include "MeStepper.h"
#include "MeEncoderMotor.h"
#include "MeEncoderNew.h"
#include "MeBuzzer.h"
#include "MeFlameSensor.h"
#include "MeGasSensor.h"*/

/*********************  101 Board GPIO Map *********************************/
// struct defined in MePort.h

MePort_Sig mePort[15] =
{
  { NC, NC }, { 11, 10 }, {  9, 12 }, { 13,  8 }, { NC,  3 },
  { NC, NC }, { NC,  2 }, { A2, A3 }, { A0, A1 }, {  5,  4 },
  {  6,  7 }, { NC, NC }, { NC, NC }, { NC, NC }, { NC, NC },
};
#define buzzerOn()  pinMode(SCL,OUTPUT),digitalWrite(SCL, HIGH)
#define buzzerOff() pinMode(SCL,OUTPUT),digitalWrite(SCL, LOW)

#endif // MeBoard101_H
