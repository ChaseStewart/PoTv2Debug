/*!
 * \file BoardLayout.hpp
 *
 * \brief Capture board pinout and physical constants for project
 *         
 * Author: Chase E. Stewart for Hidden Layer Design
 *
 */
#ifndef __BOARD_LAYOUT_HPP__
#define __BOARD_LAYOUT_HPP__

// Pinout of Teensy
#define PIN_STRUM_1070_INT  0  ///< GPIO interrupt pin for changes on StrumBoard AT42QT1070
#define PIN_STRUM_2120_INT  1  ///< GPIO interrupt pin for changes on StrumBoard AT42QT2120
#define PIN_ROT_LEDR  2  ///< Illuminated Rotary Encoder Red LED
#define PIN_ROT_LEDG  3  ///< Illuminated Rotary Encoder Green LED
#define PIN_ROT_ENC_SW  4  ///< Digital pin 4 for Rotary Encoder Switch
#define PIN_ROT_LEDB  5  ///< Illuminated Rotary Encoder Blue LED
#define PIN_ROT_ENC_C  6  ///< Illuminated Rotary Encoder "C" input for quadrature encoding
#define PIN_ROT_ENC_A  7  ///< Illuminated Rotary Encoder "A" inut for quadrature encoding
#define PIN_ULTRA_TRIG  10  ///< Trigger pin for ultrasonic rangefinder
#define PIN_ULTRA_SENS  11  ///< Sensing pin for ultrasonic rangefinder
#define PIN_ROT_POT  12  ///< Analog pin 12 for Rotary Potentiometer
#define PIN_FRET_1070_INT  14  ///< GPIO interrupt pin for changes on FretBoard AT42QT1070
#define PIN_FRET_2120_INT  15  ///< GPIO interrupt pin for changes on FretBoard AT42QT2120

#define LED_OFF    0x7  ///< 0b111 is Off, 1 corresponds to turning R/G/B channel off
#define LED_RED    0x6  ///< 0b110 is Red, only red channel on
#define LED_GREEN  0x5  ///< 0b101 is Green, only green channel on
#define LED_BLUE   0x3  ///< 0b011 is Blue, only blue channel on
#define LED_YELLOW 0x4  ///< 0b100 is Yellow, red and green channels on
#define LED_PURPLE 0x2  ///< 0b010 is Purple, red and blue channels on
#define LED_CYAN   0x1  ///< 0b001 is Cyan, green and blue channels on
#define LED_WHITE  0x0  ///< 0b000 is White, all R/G/B channels on

#endif  // __BOARD_LAYOUT_HPP__
 
