/*!
 * @file SensorState.hpp
 *
 * Author: Chase E. Stewart for Hidden Layer Design
 *
 */
#ifndef __SENSORSTATE_H__
#define __SENSORSTATE_H__

#include <stdint.h>

class SensorState 
{
  private:
    uint8_t _fret;
    uint8_t _prevFret;
    uint8_t _key;
    uint8_t _prevKey;
    bool _isRotEncSwPressed;
    uint8_t _imuX;
    uint8_t _imuY;
    uint8_t _imuZ;
    bool _isScreenUpdate;
    
  public:
    SensorState(void);
    void UpdateFret(uint8_t ks0, uint8_t ks1, uint8_t ks2);
    void UpdateStrumKey(uint8_t ss0, uint8_t ss1, uint8_t ss2);
    void checkUpdateScreen(void);
};

#endif  // __SENSORSTATE_H__
