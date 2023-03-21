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
#include "Ultrasonic.hpp"
#include "MMA8452Q.hpp"

#define PIN_ROT_ENC_A  7
#define PIN_ROT_ENC_C  6
#define PIN_ULTRA_TRIG  10
#define PIN_ULTRA_SENS  11
#define PIN_ROT_LEDR  2
#define PIN_ROT_LEDG  3
#define PIN_ROT_LEDB  5
#define ULTRASONIC_PING_PERIOD (unsigned long) (3)

#define LED_OFF    0x7 //B111
#define LED_RED    0x6 //B110
#define LED_GREEN  0x5 //B101
#define LED_BLUE   0x3 //B011
#define LED_YELLOW 0x4 //B100
#define LED_PURPLE 0x2 //B010
#define LED_CYAN   0x1 //B001
#define LED_WHITE  0x0 //B000

uint8_t strumStatus0, strumStatus1, strumStatus2;
uint8_t keyStatus0, keyStatus1, keyStatus2;

NewPing Ultrasonic = NewPing(PIN_ULTRA_TRIG, PIN_ULTRA_SENS, PITCH_BEND_MAX_CM+1);
Encoder RotaryEncoder = Encoder(PIN_ROT_ENC_A, PIN_ROT_ENC_C);
QTouchBoard fretBoard = QTouchBoard(14, 15);
QTouchBoard strumBoard = QTouchBoard(0, 1);
SensorState state = SensorState();
MMA8452Q accel;


static void pingCheck(void);
static void RotEncStandardPattern(void);


/* Ultrasonic Pitch Bend variables */
unsigned long ping_time;
unsigned long range_in_us;
unsigned long range_in_cm;


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

  accel.init();

  pinMode(PIN_ROT_LEDB, OUTPUT);
  pinMode(PIN_ROT_LEDG, OUTPUT);
  pinMode(PIN_ROT_LEDR, OUTPUT); 
  RotEncStandardPattern();

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

  state.UpdateRotPot(); 

  state.UpdateRotEncSwitch();

  // handle rotary encoder state
  if (state.UpdateRotEnc(RotaryEncoder.read()))
  {
    RotaryEncoder.write(state.GetRotEncValue());
  }

  // Get Ultrasonic Distance sensor reading
  if (micros() >= ping_time)
  {
    /* NOTE: due to using newPing timer, this has to indirectly set range_in_us */
    Ultrasonic.ping_timer(pingCheck);
    range_in_cm = range_in_us / US_ROUNDTRIP_CM;
    ping_time += ULTRASONIC_PING_PERIOD;
  }

  /* constrain range_in_cm, but sufficiently low values are treated as high ones */
  if (range_in_cm < PITCH_BEND_MIN_CM || range_in_cm > PITCH_BEND_MAX_CM)
  {
    range_in_cm = PITCH_BEND_MAX_CM;
  }
  
  state.UpdateUltrasonic(ONEBYTE_SCALED_PITCH_BEND(range_in_cm));

  /* Check Lefty Flip status */
  accel.accel_update();
  state.SetIsLeftyFlipped(accel.is_lefty_flipped());
  state.UpdateXYZ(accel.x, accel.y, accel.z);

  // if any variables changed this iter, wipe and update screen
  state.checkUpdateScreen();
  delay(100);
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


static void RotEncSetLED(uint8_t color)
{
  digitalWrite(PIN_ROT_LEDR, color & 0x1);
  digitalWrite(PIN_ROT_LEDG, color & 0x2);
  digitalWrite(PIN_ROT_LEDB, color & 0x4);
}

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
