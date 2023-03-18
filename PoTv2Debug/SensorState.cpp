/*!
 * @file SensorState.cpp
 *
 * Author: Chase E. Stewart for Hidden Layer Design
 *
 */
 #include <Wire.h>
 #include "SensorState.hpp"

/**************************************************************************/
/*!
    @brief    Creates SensorState class and set Sensor-read variables to default values
*/
/**************************************************************************/
SensorState::SensorState(void)
{
    uint8_t _fret = 0;
    uint8_t _prevFret = 0;
    uint8_t _key = 0;
    uint8_t _prevKey = 0;
    bool _isRotEncSwPressed = false;
    uint8_t _imuX = 0;
    uint8_t _imuY = 0;
    uint8_t _imuZ = 0;
    bool _isScreenUpdate = 0;
        
}

 /**************************************************************************/
/*!
    @brief    Update _fret member with the highest fret currently pressed
    @param    ks0
              Output of AT42QT2120 status register LSB
    @param    ks1
              Output of AT42QT2120 status register MSB 
    @param    ks2
              Output of AT42QT1070 status register 
*/
/**************************************************************************/
void SensorState::UpdateFret(uint8_t ks0, uint8_t ks1, uint8_t ks2)
{
  // Efficiently check FretBoard status registers for highest pressed fret
  _fret = 0;

  if (ks2)
  {
    if (ks2 & 0x40) _fret = 19;
    else if (ks2 & 0x20) _fret = 18;
    else if (ks2 & 0x10) _fret = 17;
    else if (ks2 & 0x08) _fret = 16;
    else if (ks2 & 0x04) _fret = 15;
    else if (ks2 & 0x02) _fret = 14;
    else if (ks2 & 0x01) _fret = 13;
  }
  else if (ks1)
  {
    if      (ks1 & 0x08) _fret = 12;
    else if (ks1 & 0x04) _fret = 11;
    else if (ks1 & 0x02) _fret = 10;
    else if (ks1 & 0x01) _fret = 9;
  }
  else if (ks0)
  {
    if (ks0 & 0x80) _fret = 8;
    else if (ks0 & 0x40) _fret = 7;
    else if (ks0 & 0x20) _fret = 6;
    else if (ks0 & 0x10) _fret = 5;
    else if (ks0 & 0x08) _fret = 4;
    else if (ks0 & 0x04) _fret = 3;
    else if (ks0 & 0x02) _fret = 2;
    else if (ks0 & 0x01) _fret = 1;
    else _fret = 0;
  }
  _isScreenUpdate = (_fret != _prevFret);
  _prevFret = _fret;
}

/**************************************************************************/
/*!
    @brief    Update _key member with the set of keys currently pressed, if any
    @param    ss0
              Output of AT42QT2120 status register LSB
    @param    ss1
              Output of AT42QT2120 status register MSB 
    @param    ss2
              Output of AT42QT1070 status register 
*/
/**************************************************************************/
void SensorState::UpdateStrumKey(uint8_t ss0, uint8_t ss1, uint8_t ss2)
{
  uint8_t result = 0;

  if (ss2 >> 3)
  {
    result |= 0x1;
  }
  if (((ss0 >> 5) & 0x0F) || (ss1 & 0x01))
  {
    result |= 0x2;
  } 
  if ((ss0 >> 1) & 0x0F)
  {
    result |= 0x4;
  } 
  if (((ss1 >> 1) & 0x0E) || (ss0 & 0x01))
  {
    result |= 0x8;
  } 
  _key = result;
  _isScreenUpdate = (_key != _prevKey);
  _prevKey = _key;
}

/**************************************************************************/
/*!
    @brief    Print sensor variable state in a TUI-like format
*/
/**************************************************************************/
void SensorState::checkUpdateScreen(void)
{
  if (_isScreenUpdate)
  {
    _isScreenUpdate = false;
    Serial.print('\f');
    Serial.println("+=============================================================================+");
    Serial.println("|                    * Paddle of Theseus v2 Debug Tool *                      |");
    Serial.println("|                                                                             |");
    Serial.println("|                        Written by Chase E. Stewart                          |");
    Serial.println("|                         For Hidden Layer Design                             |");
    Serial.println("+===================================+=========================================+");

    Serial.println("+===================================+=========================================+");
  }  
}
