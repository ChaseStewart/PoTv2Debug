# PoTv2Debug
**Documentation:** [![Documentation](https://github.com/adafruit/ci-arduino/blob/master/assets/doxygen_badge.svg)](https://hiddenlayerdesign.github.io/PoTv2Debug/)


This Repository holds firmware to test the second version of the Paddle of Theseus. This is a companion to the [Paddle of Theseus](https://github.com/HiddenLayerDesign/Paddle-of-Theseus) software, instead providing a debug program that uses the serial connection to characterize the sensor input for all of the sensors used in the Paddle of Theseus version 2

## Author
[Chase E Stewart](https://chasestewart.co) for [Hidden Layer Design](https://hiddenlayerdesign.com)

## Usage
This repository contains an [Arduino](https://www.arduino.cc/) project to be flashed onto a [Teensy 4.0](https://www.pjrc.com/store/teensy40.html) or [Teensy LC](https://www.pjrc.com/teensy/teensyLC.html). The target selected in Arduino (4.0 or LC) must match the position of the `SDA1_SEL` and `SCL1_SEL` jumpers on the PoT Core Module PCB to ensure proper operation.

## Note for Teensy LC 
Due to the way the Wire library is implemented for the Teensy LC, you *must* edit your WireKinetis.h file for the Teensy hardware for this software to compile. Editing WireKinetis.h is *not* required for Teensy 4.0. An example Windows path to help you find this file is `C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Wire\WireKinetis.h`

In that file, you will need to uncomment the following line
```
// Teensy LC
#if defined(__MKL26Z64__)
#define WIRE_IMPLEMENT_WIRE
//Wire1 consumes precious memory on Teensy LC
#define WIRE_IMPLEMENT_WIRE1  // <-- Uncomment this line!
#define WIRE_HAS_STOP_INTERRUPT
```
