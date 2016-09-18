/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeRGBLed
 * \brief   Driver for W2812 full-color LED.
 * @file    MeRGBLed.cpp
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/01
 * @brief   Driver for W2812 full-color LED lights
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
 * This file is a drive for WS2811/2812 full-color LED lights, It supports
 * W2812B full-color LED lights device provided by the MakeBlock.
 *
 * \par Method List:
 *
 *    1. void MeRGBLed::reset(uint8_t port)
 *    2. void MeRGBLed::reset(uint8_t port,uint8_t slot)
 *    3. void MeRGBLed::setpin(uint8_t port)
 *    4. uint8_t MeRGBLed::getNumber()
 *    5. cRGB MeRGBLed::getColorAt(uint8_t index)
 *    6. bool MeRGBLed::setColorAt(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
 *    7. bool MeRGBLed::setColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
 *    8. bool MeRGBLed::setColor(uint8_t red, uint8_t green, uint8_t blue)
 *    9. bool MeRGBLed::setColor(uint8_t index, long value)
 *    10. void MeRGBLed::show()
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/09/01     1.0.0            Rebuild the old lib.
 * </pre>
 *
 * @example ColorLoopTest.ino
 * @example IndicatorsTest.ino
 * @example WhiteBreathLightTest.ino
 */
#include "MeRGBLed.h"

#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the MeRGBLed to arduino port,
 * no pins are used or initialized here, it only assigned the LED display buffer. The default
 *number of light strips is 32.
 * \param[in]
 *   None
 */
MeRGBLed::MeRGBLed(void) : MePort()
{
  setNumber(DEFAULT_MAX_LED_NUMBER);
}

/**
 * Alternate Constructor which can call your own function to map the MeRGBLed to arduino port,
 * it will assigned the LED display buffer and initialization the GPIO of LED lights. The slot2
 * will be used here, and the default number of light strips is 32.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
MeRGBLed::MeRGBLed(uint8_t port) : MePort(port)
{
  
  _port = port;
  s2    = mePort[port].s2;
  s1    = mePort[port].s1;
  setNumber(DEFAULT_MAX_LED_NUMBER);
}

/**
 * Alternate Constructor which can call your own function to map the MeRGBLed to arduino port,
 * it will assigned the LED display buffer and initialization the GPIO of LED lights. The slot2
 * will be used here, you can reset the LED number by this constructor.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \param[in]
 *   led_num - The LED number
 */
MeRGBLed::MeRGBLed(uint8_t port, uint8_t led_num) : MePort(port)
{
  
  _port = port;
  s2    = mePort[port].s2;
  s1    = mePort[port].s1;
  setNumber(led_num);
}

/**
 * Alternate Constructor which can call your own function to map the MeRGBLed to arduino port,
 * it will assigned the LED display buffer and initialization the GPIO of LED lights. You can
 * set any slot for the LED data PIN, and reset the LED number by this constructor.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \param[in]
 *   slot - SLOT1 or SLOT2
 * \param[in]
 *   led_num - The LED number
 */
MeRGBLed::MeRGBLed(uint8_t port, uint8_t slot, uint8_t led_num) : MePort(port)
{
  
  _port = port;
  s2    = mePort[port].s2;
  s1    = mePort[port].s1;
  if(SLOT2 == slot)
  {
    
	s2    = mePort[port].s2;
  }
  else
  {
	s2    = mePort[port].s1;
  }
  setNumber(led_num);
}
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the MeRGBLed to arduino port,
 * it will assigned the LED display buffer and initialization the GPIO of LED lights. You can
 * set any arduino digital pin for the LED data PIN, The default number of light strips is 32.
 * \param[in]
 *   port - arduino port
 */
MeRGBLed::MeRGBLed(uint8_t port)
{
 
  _port = port;
  s2    = mePort[port].s2;
  s1    = mePort[port].s1;
  setNumber(DEFAULT_MAX_LED_NUMBER);
}

/**
 * Alternate Constructor which can call your own function to map the MeRGBLed to arduino port,
 * it will assigned the LED display buffer and initialization the GPIO of LED lights. You can
 * set any arduino digital pin for the LED data PIN, and reset the LED number by this constructor.
 * \param[in]
 *   port - arduino port
 * \param[in]
 *   led_num - The LED number
 */
MeRGBLed::MeRGBLed(uint8_t port, uint8_t led_num)
{
  _port = port;
  s2    = mePort[port].s2;
  s1    = mePort[port].s1;
  setNumber(led_num);
}
#endif // ME_PORT_DEFINED

#ifdef ME_PORT_DEFINED
/**
 * \par Function
 *   reset
 * \par Description
 *   Reset the LED available data PIN by its RJ25 port, and slot2 will be used as default.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeRGBLed::reset(uint8_t port)
{
  _port = port;
  s2    = mePort[port].s2;
  s1    = mePort[port].s1;
  setNumber(count_led);
}

/**
 * \par Function
 *   reset
 * \par Description
 *   Reset the LED available data PIN by its RJ25 port and slot.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \param[in]
 *   slot - SLOT1 or SLOT2
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeRGBLed::reset(uint8_t port,uint8_t slot)
{
  _port = port;
  s2    = mePort[port].s2;
  s1    = mePort[port].s1;
  if(SLOT2 == slot)
  {
    
	s2    = mePort[port].s2;
  }
  else
  {
	s2    = mePort[port].s1;
  }
  setNumber(count_led);
}
#endif //ME_PORT_DEFINED
/**
 * \par Function
 *   setpin
 * \par Description
 *   Reset the LED available data PIN by its arduino port.
 * \param[in]
 *   port - arduino port(should digital pin)
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeRGBLed::setpin(uint8_t port)
{
  s2 = port;
}

/**
 * \par Function
 *   setNumber
 * \par Description
 *   Assigned the LED display buffer by the LED number
 * \param[in]
 *   num_leds - The LED number you used
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeRGBLed::setNumber(uint8_t num_leds)
{
  count_led = num_leds;
  led = Adafruit_NeoPixel(count_led,s2, NEO_GRB + NEO_KHZ800);
  led.begin();
}

/**
 * \par Function
 *   getColorAt
 * \par Description
 *   Get the LED color value from its index
 * \param[in]
 *   index - The LED index number you want to read its value
 * \par Output
 *   None
 * \return
 *   The LED color value, include the R,G,B
 * \par Others
 *   The index value from 1 to the max
 */
cRGB MeRGBLed::getColorAt(uint8_t index)
{
	cRGB px_value;
  if(index < count_led)
  {
    uint32_t tmp = led.getPixelColor(index);
	uint8_t red    = (tmp & 0xff0000) >> 16;
    uint8_t green  = (tmp & 0xff00) >> 8;
    uint8_t blue   = tmp & 0xff;
    px_value.r = red;
    px_value.g = green;
    px_value.b = blue;
  }
  return(px_value);
}

/**
 * \par Function
 *   getNumber
 * \par Description
 *   Get the LED number you can light it.
 * \par Output
 *   None
 * \return
 *   The total number of LED's
 * \par Others
 *   The index value from 1 to the max
 */
uint8_t MeRGBLed::getNumber(void)
{
  return(count_led);
}

/**
 * \par Function
 *   setColorAt
 * \par Description
 *   Set the LED color for any LED.
 * \param[in]
 *   index - The LED index number you want to set its color
 * \param[in]
 *   red - Red values
 * \param[in]
 *   green - green values
 * \param[in]
 *   blue - blue values
 * \par Output
 *   None
 * \return
 *   TRUE: Successful implementation
 *   FALSE: Wrong execution
 * \par Others
 *   The index value from 0 to the max.
 */
bool MeRGBLed::setColorAt(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
{
  if(index < count_led)
  {
    
    led.setPixelColor(index,red,green,blue);
    return(true);
  }
  return(false);
}

/**
 * \par Function
 *   setColor
 * \par Description
 *   Set the LED color for any LED.
 * \param[in]
 *   index - The LED index number you want to set its color
 * \param[in]
 *   red - Red values
 * \param[in]
 *   green - green values
 * \param[in]
 *   blue - blue values
 * \par Output
 *   None
 * \return
 *   TRUE: Successful implementation
 *   FALSE: Wrong execution
 * \par Others
 *   The index value from 1 to the max, if you set the index 0, all the LED will be lit
 */
bool MeRGBLed::setColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
{
  if(index == 0)
  {
    for(int16_t i = 0; i < count_led; i++)
    {
      setColorAt(i,red,green,blue);
    }
    return(true);
  }
  else
  {
    setColorAt(index-1,red,green,blue);
  }
  return(false);
}

/**
 * \par Function
 *   setColor
 * \par Description
 *   Set the LED color for all LED.
 * \param[in]
 *   red - Red values
 * \param[in]
 *   green - green values
 * \param[in]
 *   blue - blue values
 * \par Output
 *   None
 * \return
 *   TRUE: Successful implementation
 *   FALSE: Wrong execution
 * \par Others
 *   All the LED will be lit.
 */
bool MeRGBLed::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
  return(setColor(0, red, green, blue) );;
}

/**
 * \par Function
 *   setColor
 * \par Description
 *   Set the LED color for any LED.
 * \param[in]
 *   value - the LED color defined as long type, for example (white) = 0xFFFFFF
 * \par Output
 *   None
 * \return
 *   TRUE: Successful implementation
 *   FALSE: Wrong execution
 * \par Others
 *   The index value from 1 to the max, if you set the index 0, all the LED will be lit
 */
bool MeRGBLed::setColor(uint8_t index, long value)
{
    uint8_t red    = (value & 0xff0000) >> 16;
    uint8_t green  = (value & 0xff00) >> 8;
    uint8_t blue   = value & 0xff;
  if(index < count_led)
  {
	
    setColor(index,red,green,blue);
    return(true);
  }
  return(false);
}


/**
 * \par Function
 *   show
 * \par Description
 *   Transmission the data to WS2812
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeRGBLed::show(void)
{
  led.show();
}

/**
 * Destructor which can call your own function, it will release the LED buffer
 */
MeRGBLed::~MeRGBLed(void)
{
}

