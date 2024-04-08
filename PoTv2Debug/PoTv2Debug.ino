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
#include "BoardLayout.hpp"
#include "QTouchBoard.hpp"
#include "SensorState.hpp"
#include "Ultrasonic.hpp"
#include "MMA8452Q.hpp"

NewPing Ultrasonic = NewPing(PIN_ULTRA_TRIG, PIN_ULTRA_SENS, PITCH_BEND_MAX_CM+1);
Encoder RotaryEncoder = Encoder(PIN_ROT_ENC_A, PIN_ROT_ENC_C);
QTouchBoard fretBoard = QTouchBoard(PIN_FRET_1070_INT, PIN_FRET_2120_INT);
QTouchBoard strumBoard = QTouchBoard(PIN_STRUM_1070_INT, PIN_STRUM_2120_INT);
SensorState state = SensorState();
MMA8452Q accel;

static void pingCheck(void);
static void RotEncSetLED(uint8_t color);
static void RotEncStandardPattern(void);

// utrasonic variables
unsigned long ping_time;
unsigned long range_in_us;
unsigned long range_in_cm;

// QTouchBoard variables
uint8_t strumStatus0, strumStatus1, strumStatus2;
uint8_t keyStatus0, keyStatus1, keyStatus2;

// Rotary Encoder read variable
int32_t rotEncRetval;

/**************************************************************************/
/*!
    @brief    Instantiate Serial connection and setup hardware and ports/pins
*/
/**************************************************************************/
void setup() 
{
  Serial.begin(500000);
  delay(1000);
  
  Serial.println("*** Paddle of Theseus Test Software v2 ***");
  Serial.println();

  Serial.println("*** Setup FretBoard ***");
  Wire.begin();
  fretBoard.begin(Wire);
  Serial.println("*** Done ***");

  Serial.println("*** Setup StrumBoard ***");
  Wire1.begin();
  strumBoard.begin(Wire1);
  Serial.println("*** Done ***");

  accel.init();

  pinMode(PIN_ROT_LEDB, OUTPUT);
  pinMode(PIN_ROT_LEDG, OUTPUT);
  pinMode(PIN_ROT_LEDR, OUTPUT); 
  RotEncStandardPattern();

  delay(1000);

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
    keyStatus0 = fretBoard.QT2120ReadSingleReg(REG_QT2120_KEY_STATUS_0);
    keyStatus1 = fretBoard.QT2120ReadSingleReg(REG_QT2120_KEY_STATUS_1);
    keyStatus2 = fretBoard.QT1070ReadSingleReg(REG_QT1070_KEY_STATUS_0);
    state.UpdateFret(keyStatus0, keyStatus1, keyStatus2);
  }

  if (strumBoard.isValueUpdate())
  {
    strumStatus0 = strumBoard.QT2120ReadSingleReg(REG_QT2120_KEY_STATUS_0);
    strumStatus1 = strumBoard.QT2120ReadSingleReg(REG_QT2120_KEY_STATUS_1);
    strumStatus2 = strumBoard.QT1070ReadSingleReg(REG_QT1070_KEY_STATUS_0);
    state.UpdateStrumKey(strumStatus0, strumStatus1, strumStatus2);
  }

  state.UpdateRotPot(); 
  state.UpdateRotEncSwitch();

  // handle rotary encoder state
  rotEncRetval = state.ProcessRotEnc(RotaryEncoder.read()); 
  RotaryEncoder.write( (state.GetIsLeftyFlipped()) ? 
                       (-1 *rotEncRetval) : rotEncRetval);
  state.UpdateRotEnc((uint8_t) rotEncRetval);

  // Get Ultrasonic Distance sensor reading
  if (micros() >= ping_time)
  {
    // due to using newPing timer, this has to indirectly set range_in_us
    Ultrasonic.ping_timer(pingCheck);
    range_in_cm = range_in_us / US_ROUNDTRIP_CM; // NOTE this US_ROUNDTRIP_CM is in NewPing source code 
    ping_time += ULTRASONIC_PING_PERIOD_MICROS;
  }

  // constrain range_in_cm, but sufficiently low values are treated as high ones
  if (range_in_cm < PITCH_BEND_MIN_CM || range_in_cm > PITCH_BEND_MAX_CM)
  {
    range_in_cm = PITCH_BEND_MAX_CM;
  }
  
  state.UpdateUltrasonic(ONEBYTE_SCALED_PITCH_BEND(range_in_cm));

  // Check Lefty Flip status
  accel.Update();
  state.SetIsLeftyFlipped(accel.IsLeftyFlipped());
  state.UpdateXYZ(accel.x, accel.y, accel.z);

  // if any variables changed this iter, wipe and update screen
  state.CheckUpdateScreen();
  delay(100); //
}

/**************************************************************************/
/*!
    @brief    Callback function to check whether ultrasonic sonar has returned data
*/
/**************************************************************************/
static void pingCheck(void)
{
  range_in_us = (Ultrasonic.check_timer()) ? Ultrasonic.ping_result : range_in_us +2;
}

/**************************************************************************/
/*!
    @brief    Set the LED on the Illuminated Rotary Encoder
    @param    color
              One of the color values defined in BoardLayout.hpp, a bitmap to RGB on/off    
*/
/**************************************************************************/
static void RotEncSetLED(uint8_t color)
{
  digitalWrite(PIN_ROT_LEDR, color & 0x1);
  digitalWrite(PIN_ROT_LEDG, color & 0x2);
  digitalWrite(PIN_ROT_LEDB, color & 0x4);
}

/**************************************************************************/
/*!
    @brief    Blocking function to display a sort of rainbow color pattern on Illuminated Rotary Encoder
*/
/**************************************************************************/
static void RotEncStandardPattern(void)
{
  RotEncSetLED(LED_BLUE);
  delay(500);  
  RotEncSetLED(LED_PURPLE);
  delay(500);  
  RotEncSetLED(LED_GREEN);
  delay(500);  
  RotEncSetLED(LED_CYAN);
  delay(500);  
  RotEncSetLED(LED_RED);
  delay(500);  
  RotEncSetLED(LED_YELLOW);
  delay(500);  
  RotEncSetLED(LED_WHITE);
  delay(2000);  
}
