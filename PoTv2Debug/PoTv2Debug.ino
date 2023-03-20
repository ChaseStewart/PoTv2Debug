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
#include <Encoder.h>
#include <NewPing.h>
#include "QTouchBoard.hpp"
#include "SensorState.hpp"

#define PIN_ROT_ENC_A 7
#define PIN_ROT_ENC_C 6

uint8_t strumStatus0, strumStatus1, strumStatus2;
uint8_t keyStatus0, keyStatus1, keyStatus2;

Encoder RotaryEncoder = Encoder(PIN_ROT_ENC_A, PIN_ROT_ENC_C);
QTouchBoard fretBoard = QTouchBoard(14, 15);
QTouchBoard strumBoard = QTouchBoard(0, 1);
SensorState state = SensorState();

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
    state.UpdateFret(keyStatus0, keyStatus1, keyStatus2);
  }

  if (strumBoard.isValueUpdate())
  {
    strumStatus0 = strumBoard.QT2120ReadSingleReg(3);
    strumStatus1 = strumBoard.QT2120ReadSingleReg(4);
    strumStatus2 = strumBoard.QT1070ReadSingleReg(3);
    state.UpdateStrumKey(strumStatus0, strumStatus1, strumStatus2);
  }

  //state.UpdateRotPot(); // TODO needs HW mod on CoreModule 2.0 
  state.UpdateRotEncSwitch();

  if (state.UpdateRotEnc(RotaryEncoder.read()))
  {
    RotaryEncoder.write(state.GetRotEncValue());
  }
  
  // if any variables changed this iter, wipe and update screen
  state.checkUpdateScreen();
}
