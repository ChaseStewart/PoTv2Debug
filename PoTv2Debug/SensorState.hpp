/*!
 * @file SensorState.hpp
 *
 * Author: Chase E. Stewart for Hidden Layer Design
 *
 */
#ifndef __SENSORSTATE_H__
#define __SENSORSTATE_H__

#include <stdint.h>

/**************************************************************************/
/*!
    @brief  Class to keep and display all sensor values
    Displaying sensor values is the primary goal of this project, so
    the data conversions and checking for new values can be abstracted into
    this class rather than busying up the main loop with globals and logic
*/
/**************************************************************************/
class SensorState 
{
  private:
    uint8_t _fret;  ///< Number of fret currently pressed
    uint8_t _prevFret;  ///< Number of fret pressed last loop iter
    uint8_t _key;  ///< Bitfield of pressed keys
    uint8_t _prevKey;  ///< Bitfield of pressed keys from last loop iter
    bool _rotEncSwitch;  ///< True if rotEnc switch currently pressed, else False
    uint8_t _rotEnc;  ///< Value of rotary encoder
    uint8_t _rotPot;  ///< Value of rotary
    uint8_t _ultraDist;  ///< Distance in cm from ultrasonic rangefinder
    uint8_t _imuX;  ///< X-value of IMU
    uint8_t _imuY;  ///< Y-value of IMU
    uint8_t _imuZ;  ///< Z-value of IMU
    bool _isScreenUpdate;  ///< True if screen should be updated this iter, else False
    bool _isLefty;  ///< True if Lefty mode should be enabled this Iter, else False
    void _printUint8_t(uint8_t value);
    
  public:
    SensorState(void);
    void UpdateFret(uint8_t ks0, uint8_t ks1, uint8_t ks2);
    void UpdateStrumKey(uint8_t ss0, uint8_t ss1, uint8_t ss2);
    void checkUpdateScreen(void);
};

#endif  // __SENSORSTATE_H__
