/*!
 * @file PoTv2Debug.ino
 *
 * @mainpage Paddle of Theseus v2 Debug TUI for serial connection
 *
 * @section intro_sec Introduction
 *
 * This is the documentation for the Paddle of Theseus v2 Debug tool 
 * It allows a user to verify proper sensing of Paddle v2 inputs using 
 * a serial connection to the Teensy at 115200 baud.
 *
 * @section requirements Requirements
 *
 * This software requires a full hardware Paddle of Theseus v2 <a
 * href="https://github.com/arduino-libraries/ArduinoHttpClient">
 * and also if a Teensy LC is used, requires modifications to 
 * the Arduino hardware library file WireKinetis.h for the Teensy Hardware
 * in order to enable Wire1. See https://github.com/ChaseStewart/PoTv2Debug#readme
 * for more details.
 *
 * @section author Author
 *
 * Written by <a href="https://chasestewart.co"> 
 * Chase E. Stewart</a> for <a href="https://hiddenlayerdesign.com">
 * Hidden Layer Design</a>
 * 
 */
#include <Wire.h>
#include "QTouchBoard.hpp"

static uint8_t GetFret(uint8_t ks0, uint8_t ks1, uint8_t ks2);
static uint8_t GetStrumKey(uint8_t ss0, uint8_t ss1, uint8_t ss2);

uint8_t fret;  ///< The highest currently-pressed fret, mimics a guitar fretboard
uint8_t prevFret;
uint8_t strumStatus0, strumStatus1, strumStatus2;

uint16_t key;  ///< Bitfield of the key or keys that have been pressed 
uint16_t prevKey;
uint8_t keyStatus0, keyStatus1, keyStatus2;

bool screenUpdate;  ///< when True, update the serial screen with current variable values 

QTouchBoard fretBoard = QTouchBoard(14, 15);
QTouchBoard strumBoard = QTouchBoard(0, 1);

/**************************************************************************/
/*!
    @brief    Instantiate Serial connection and setup hardware and ports/pins
*/
/**************************************************************************/
void setup() 
{
  Serial.begin(500000);
  
  Serial.println("*** Paddle of Theseus Fretboard Test v2 ***");
  Serial.println();

  Serial.println("*** Setup FretBoard ***");
  Wire.begin();
  fretBoard.begin(Wire);
  Serial.println("*** Done ***");

  Serial.println("*** Setup StrumBoard ***");
  Wire1.begin();
  strumBoard.begin(Wire1);
  Serial.println("*** Done ***");

  delay(2000);
  fret = 0;

  Serial.println();
}

/**************************************************************************/
/*!
    @brief    Poll sensors, detect changes, and update Serial UI when variables update
*/
/**************************************************************************/
void loop()
{
  if (fretBoard.isValueUpdate())
  {    
    keyStatus0 = fretBoard.QT2120ReadSingleReg(3);
    keyStatus1 = fretBoard.QT2120ReadSingleReg(4);
    keyStatus2 = fretBoard.QT1070ReadSingleReg(3);
    fret = GetFret(keyStatus0, keyStatus1, keyStatus2);
    if (prevFret != fret)
    {
      screenUpdate = true;
    }
    prevFret = fret;
  }

  if (strumBoard.isValueUpdate())
  {
    strumStatus0 = strumBoard.QT2120ReadSingleReg(3);
    strumStatus1 = strumBoard.QT2120ReadSingleReg(4);
    strumStatus2 = strumBoard.QT1070ReadSingleReg(3);
    key = GetStrumKey(strumStatus0, strumStatus1, strumStatus2);
    if (prevKey != key)
    {
      screenUpdate = true;
    }
    prevKey = key;
  }

  // if any variables changed this iter, wipe and update screen
  if (screenUpdate)
  {
    screenUpdate = false;
    Serial.print('\f');
    Serial.println("+=============================================================================+");
    Serial.println("|                    * Paddle of Theseus v2 Debug Tool *                      |");
    Serial.println("|                                                                             |");
    Serial.println("|                        Written by Chase E. Stewart                          |");
    Serial.println("|                         For Hidden Layer Design                             |");
    Serial.println("+===================================+=========================================+");
    Serial.print("| Curr Fret:"); Serial.print((fret < 10) ? " 0" : " "); Serial.print(fret, DEC); 
    Serial.print("/ 19                 | ");
    Serial.print("Keys Pressed  3:["); Serial.print((key & 0x8) ? 'x' : ' '); Serial.print("] 2:["); 
    Serial.print((key & 0x4) ? 'x' : ' '); Serial.print("] 1:["); Serial.print((key & 0x2) ? 'x' : ' '); 
    Serial.print("] 0:["); Serial.print((key & 0x1) ? 'x' : ' '); Serial.println("]   |");
    Serial.println("+===================================+=========================================+");

#if 0
    Serial.print("Strum=");    Serial.print(key, HEX); 
    Serial.print(", ss2=0x"); Serial.println(strumStatus2, HEX);
    Serial.print(", ss1=0x"); Serial.print(strumStatus1, HEX);
    Serial.print(", ss0=0x"); Serial.print(strumStatus0, HEX); 
    Serial.println();

    Serial.print("Fret=");    Serial.print(fret); 
    Serial.print(", ks2=0x"); Serial.println(keyStatus2, HEX);  
    Serial.print(", ks1=0x"); Serial.print(keyStatus1, HEX);
    Serial.print(", ks0=0x"); Serial.print(keyStatus0, HEX); 
#endif // if 0
  }
}


/**************************************************************************/
/*!
    @brief    Return the set of keys currently pressed, if any
    @param    ss0
              Output of AT42QT2120 status register LSB
    @param    ss1
              Output of AT42QT2120 status register MSB 
    @param    ss2
              Output of AT42QT1070 status register 
    @return   integer strum value representing the key or keys currently pressed, if any
*/
/**************************************************************************/
static uint8_t GetStrumKey(uint8_t ss0, uint8_t ss1, uint8_t ss2)
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
  return result;
}

/**************************************************************************/
/*!
    @brief    Return the highest fret currently pressed
    @param    ks0
              Output of AT42QT2120 status register LSB
    @param    ks1
              Output of AT42QT2120 status register MSB 
    @param    ks2
              Output of AT42QT1070 status register 
    @return   integer fret value representing the highest fret currently pressed
*/
/**************************************************************************/
static uint8_t GetFret(uint8_t ks0, uint8_t ks1, uint8_t ks2)
{
  // Efficiently check FretBoard status registers for highest pressed fret
  if (ks2)
  {
    if (ks2 & 0x40) return 19;
    else if (ks2 & 0x20) return 18;
    else if (ks2 & 0x10) return 17;
    else if (ks2 & 0x08) return 16;
    else if (ks2 & 0x04) return 15;
    else if (ks2 & 0x02) return 14;
    else if (ks2 & 0x01) return 13;
  }
  if (ks1)
  {
    if      (ks1 & 0x08) return 12;
    else if (ks1 & 0x04) return 11;
    else if (ks1 & 0x02) return 10;
    else if (ks1 & 0x01) return 9;
  }
  if (ks0)
  {
    if (ks0 & 0x80) return 8;
    else if (ks0 & 0x40) return 7;
    else if (ks0 & 0x20) return 6;
    else if (ks0 & 0x10) return 5;
    else if (ks0 & 0x08) return 4;
    else if (ks0 & 0x04) return 3;
    else if (ks0 & 0x02) return 2;
    else if (ks0 & 0x01) return 1;
    else return 0;
  }
  return 0;
}
