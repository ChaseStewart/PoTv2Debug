/*!
 * @file SensorState.cpp
 *
 * \brief Class to hold and update sensor values and display them over serial
 *        As mentioned in CheckUpdateScreen's header, updating and displaying sensor values
 *        over the serial port is the main function of this software
 *
 * Author: Chase E. Stewart for Hidden Layer Design
 *
 */
#include <Wire.h>
#include "SensorState.hpp"
#include "Ultrasonic.hpp"


/**************************************************************************/
/*!
    @brief    Create SensorState class and set variables to default values
*/
/**************************************************************************/
SensorState::SensorState(void)
{
  
  _fret = 0;
  _prevFret = 0;
  _key = 0;
  _prevKey = 0;
  _rotEncSwitch = false;
  _prevRotEncSwitch = false;
  _rotEnc = 0;
  _prevRotEnc = 0;
  _rotPot = 0;
  _prevRotPot = 0;
  _ultraDist = 0;
  _prevUltraDist = 0;
  _imuX = 0;
  _imuY = 0;
  _imuZ = 0;
  _isScreenUpdate = false;
  _isLefty = false;  
   pinMode(PIN_ROT_POT, INPUT);      
   pinMode(PIN_ROT_ENC_SW, INPUT);      
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
  if (_fret != _prevFret)
  {
    _isScreenUpdate = true;
    _prevFret = _fret;
  }
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
  if (_key != _prevKey);
  {
    _isScreenUpdate = true;
    _prevKey = _key;
  }
}

/**************************************************************************/
/*!
    @brief    Read voltage val of rotary potentiometer and store value
*/
/**************************************************************************/
void SensorState::UpdateRotPot(void)
{
  _rotPot = (_isLefty) ? floor(analogRead(PIN_ROT_POT) * 256.0/1024.0) :  floor((1024 - analogRead(PIN_ROT_POT)) * 128.0/1024.0);
  if (_rotPot != _prevRotPot)
  {
    _isScreenUpdate = true;
    _prevRotPot = _rotPot;
  }  
}

/**************************************************************************/
/*!
    @brief    Store value of Rotary Encoder switch
*/
/**************************************************************************/
void SensorState::UpdateRotEncSwitch(void)
{
  _rotEncSwitch = digitalRead(PIN_ROT_ENC_SW);
  if (_rotEncSwitch != _prevRotEncSwitch)
  {
    _isScreenUpdate = true;
    _prevRotEncSwitch = _rotEncSwitch;
  }
}

/**************************************************************************/
/*!
    @brief    Store value of Rotary Encoder knob
*/
/**************************************************************************/
bool SensorState::UpdateRotEnc(uint8_t newValue)
{
  uint8_t constrainedRotEnc;
   
  /* Sample Rotary Encoder Twist Knob */
  if (_isLefty)
  {
    _rotEnc =  _prevRotEnc + (-1 * (newValue - _prevRotEnc));
  }
  else
  {
    _rotEnc = newValue;
  }

  constrainedRotEnc = constrain(_rotEnc, ROT_ENC_MIN, ROT_ENC_MAX);
  if (constrainedRotEnc != _prevRotEnc)
  {
    _isScreenUpdate = true;
    _prevRotEnc = constrainedRotEnc;
    return true;
  }
  return false;
}

void SensorState::UpdateUltrasonic(uint8_t newValue)
{
  _ultraDist = newValue;
  if (_ultraDist != _prevUltraDist && abs(_ultraDist - _prevUltraDist) < MAX_PITCH_BEND_DELTA)
  {
    _isScreenUpdate = true;
    _prevUltraDist = _ultraDist;
  }
}


/**************************************************************************/
/*!
    @brief    Set whether lefty mode is enabled
*/
/**************************************************************************/
void SensorState::SetIsLeftyFlipped(bool isFlipped)
{
  _isLefty = isFlipped;
  if (_isLefty != _prevIsLefty)
  {
    _isScreenUpdate = true;
    _prevIsLefty = _isLefty;  
  }
}

/**************************************************************************/
/*!
    @brief    Set whether lefty mode is enabled
*/
/**************************************************************************/
void SensorState::UpdateXYZ(uint8_t x, uint8_t y, uint8_t z)
{
  _imuX = x;
  _imuY = y;
  _imuZ = z;

  if ((_imuX != _prevImuX) || (_imuY != _prevImuY) || (_imuZ != _prevImuZ))
  {
    _isScreenUpdate = true;
    _prevImuX = _imuX;
    _prevImuY = _imuY;
    _prevImuZ = _imuZ;    
  }
}


/**************************************************************************/
/*!
    @brief    Store value of Rotary Encoder knob
*/
/**************************************************************************/
uint8_t SensorState::GetRotEncValue(void)
{
  return _rotEnc;
}

/**************************************************************************/
/*!
    @brief    Convenience function equivalent of %02u
    @param    value
              byte integer to print 
*/
/**************************************************************************/
void SensorState::_printUint8_t(uint8_t value)
{
  Serial.print((value < 100) ? " " : ""); 
  Serial.print((value < 10) ? "0" : ""); 
  Serial.print(value, DEC);
}

/**************************************************************************/
/*!
    @brief    Print sensor variable state in a TUI-like format
    
This serial output is designed to be the only output on the screen
and provide a user interface as opposed to a logfile format.
The goal is to re-write this full output as quickly as possible whenever
a variable changes, thus providing a responsive interface that lets a user
benchmark the sensors by comparing printed screen values with sensor input
*/
/**************************************************************************/
void SensorState::CheckUpdateScreen(void)
{
  if (_isScreenUpdate)
  {
    _isScreenUpdate = false;
    Serial.print('\f');  // Wipe the screen output on compliant terminals before re-writing
    Serial.println("+=============================================================================+");
    Serial.println("|                    * Paddle of Theseus v2 Debug Tool *                      |");
    Serial.println("|                                                                             |");
    Serial.println("|                        Written by Chase E. Stewart                          |");
    Serial.println("|                         For Hidden Layer Design                             |");
    Serial.println("|                                                                             |");
    Serial.println("+=============================================================================+");
    Serial.print("| Curr Fret:"); _printUint8_t(_fret);
    Serial.print("/ 19                 | ");
    Serial.print("Keys Pressed  3:["); Serial.print((_key & 0x8) ? 'x' : ' '); Serial.print("] 2:["); 
    Serial.print((_key & 0x4) ? 'x' : ' '); Serial.print("] 1:["); Serial.print((_key & 0x2) ? 'x' : ' '); 
    Serial.print("] 0:["); Serial.print((_key & 0x1) ? 'x' : ' '); Serial.println("]   |");
    Serial.println("+-----------------------------------+-----------------------------------------+");
    Serial.print("| RotEnc Value: "); _printUint8_t(_rotEnc);
    Serial.print(" RotEnc SW: ["); Serial.print((_rotEncSwitch) ? 'x' : ' '); Serial.print("]  | Potentiometer value:");
    _printUint8_t(_rotPot); Serial.println("/128             |");
    Serial.println("+-----------------------------------+-----------------------------------------+");
    Serial.print("| IMU x:"); _printUint8_t(_imuX);
    Serial.print(" y:"); _printUint8_t(_imuY);
    Serial.print(" z:"); _printUint8_t(_imuZ);
    Serial.print("  Lefty: ["); Serial.print((_isLefty) ? 'x' : ' ');
    Serial.print("] | Ultrasonic Distance: "); _printUint8_t(_ultraDist);
    Serial.println("                |");
    Serial.println("+===================================+=========================================+");
  }  
}
