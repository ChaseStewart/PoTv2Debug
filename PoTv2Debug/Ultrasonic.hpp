/*!
 * @file Ultrasonic.hpp
 *
 * \brief Header containing Ultrasonic Rangefinder config and macros
 * 
 * Author: Chase E. Stewart for Hidden Layer Design
 *
 */
#ifndef __ULTRASONIC_HPP__
#define __ULTRASONIC_HPP__

// This file is now very barebones because the NewPing library is doing the heavy-lifting for the Ultrasonic firmware 

#define PITCH_BEND_MAX_CM 60  ///< Max distance in CM that will be measured by pitch bend 
                              ///< Distances beyond this will be treated as "no signal"
#define PITCH_BEND_MIN_CM 1  ///< Min distance in CM that will be measured by pitch bend 
#define ULTRASONIC_PING_PERIOD_MICROS (unsigned long) (3)  ///< How often to trigger the ultrasonic sensor 
#define MAX_PITCH_BEND_DELTA 1700  ///< Max value that pitch bend can change by without getting thrown out as anomalous
                                   ///< This is a unitless raw reading


///< \def SCALED_PITCH_BEND(x)
///< MIDI Pitch bend message accepts a 14-bit twos compliment value, 
///< but we only want to detune like a whammy bar, so we use 12 bits and subtract 2^12 
///< such that max val is 0 and whammy goes to half of the full negative (detune) range 
///< NOTE: use pow(2,13) instead for full range!
#define SCALED_PITCH_BEND(x) (int) (pow(2,12) * x  / PITCH_BEND_MAX_CM) - pow(2,12)  // Do not adjust!

///< \def ONEBYTE_SCALED_PITCH_BEND(x)
///< If using pitch bend for another MIDI control code, the range is only 0-127
#define ONEBYTE_SCALED_PITCH_BEND(x) (int) (pow(2,7) * (PITCH_BEND_MAX_CM - x)  / PITCH_BEND_MAX_CM)  // Do not adjust!


#endif // __ULTRASONIC_HPP__
