/*!
 * \file SensorState.hpp
 *
 * \brief Header for class to hold and update sensor values and display them over serial
 *
 * Author: Chase E. Stewart for Hidden Layer Design
 *
 */
#ifndef __SENSORSTATE_HPP__
#define __SENSORSTATE_HPP__

#include <stdint.h>
#include "BoardLayout.hpp"


#define ROT_ENC_MIN 0  ///< Minimum value to constrain rotaryEncoder reading
#define ROT_ENC_MAX 127  ///< Maximum value to constrain rotaryEncoder reading

/**************************************************************************/
/*!
    \brief  Class to keep and display all sensor values
    Displaying sensor values is the primary goal of this project, so
    the data conversions and checking for new values can be abstracted into
    this class rather than busying up the main loop with globals and logic
*/
/**************************************************************************/
class SensorState 
{
  ///< \todo turn this giant list of member vars to a pair of current/ prev structs
  private:
    uint8_t _fret;  ///< Number of fret currently pressed
    uint8_t _prevFret;  ///< Number of fret pressed last loop iter
    uint8_t _key;  ///< Bitfield of pressed keys
    uint8_t _prevKey;  ///< Bitfield of pressed keys from last loop iter
    bool _rotEncSwitch;  ///< True if rotEnc switch currently pressed, else False
    bool _prevRotEncSwitch;  ///< True if rotEnc switch pressed last loop iter, else False
    uint8_t _rotEnc;  ///< Value of rotary encoder
    uint8_t _prevRotEnc;  ///< Value of rotary encoder last loop iter
    uint8_t _rotPot;  ///< Value of rotary potentiometer
    uint8_t _prevRotPot;  ///< Value of rotary potentiometer
    uint8_t _ultraDist;  ///< Distance in cm from ultrasonic rangefinder
    uint8_t _prevUltraDist;  ///< Distance in cm from ultrasonic rangefinder
    uint8_t _imuX;  ///< X-value of IMU
    uint8_t _prevImuX;  ///< X-value of IMU last loop iter
    uint8_t _imuY;  ///< Y-value of IMU
    uint8_t _prevImuY;    ///< Y-value of IMU last loop iter
    uint8_t _imuZ;  ///< Z-value of IMU
    uint8_t _prevImuZ;  ///< Z-value of IMU last loop iter
    bool _isScreenUpdate;  ///< True if screen should be updated this iter, else False
    bool _isLefty;  ///< True if Lefty mode is enabled this Iter, else False
    bool _prevIsLefty;  ///< True if Lefty mode was enabled last loop Iter, else False
    
    void _printUint8_t(uint8_t value);
    
  public:
    SensorState(void);
    void UpdateFret(uint8_t ks0, uint8_t ks1, uint8_t ks2);
    void UpdateRotPot(void);
    void UpdateRotEncSwitch(void);
    bool UpdateRotEnc(uint8_t newValue);
    uint8_t GetRotEncValue(void);
    void UpdateStrumKey(uint8_t ss0, uint8_t ss1, uint8_t ss2);
    void UpdateUltrasonic(uint8_t newValue);
    void CheckUpdateScreen(void);
    void SetIsLeftyFlipped(bool isFlipped);
    void UpdateXYZ(uint8_t x, uint8_t y, uint8_t z);
};

#endif  // __SENSORSTATE_HPP__
