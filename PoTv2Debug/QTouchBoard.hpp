/*!
 * @file QTouchBoard.hpp
 *
 * Author: Chase E. Stewart for Hidden Layer Design
 *
 */
#ifndef __QTOUCHBOARD_H__
#define __QTOUCHBOARD_H__

#include <Wire.h>

#define QTOUCH2120_ADDR  0x1C  ///< Static I2C address for AT42QT2120 part
#define QTOUCH1070_ADDR  0x1B  ///< Static I2C address for AT42QT2120 part
#define OFST_QT1070_INTEGRATION_REG  46
#define OFST_QT1070_AVE_AKS_REG  39

/**************************************************************************/
/*!
    @brief  Class for interacting with QTouch I2C devices
    This class interfaces with the HiddenLayerDesign custom QTouch-powered I2C 
    boards on Wire and Wire1 used for the FretBoard and StrumBoard. 
    Despite their unique layouts, the schematics and partlists of the two boards 
    are identical, so making a common class saves a ton of code
*/
/**************************************************************************/
class QTouchBoard 
{
  private:
    TwoWire *_i2cStream;  ///< One of Wire/ Wire1 class for I2C
    int _intPin1070;  ///< GPIO interrupt pin for AT42QT1070
    int _intPin2120;  ///< GPIO interrupt pin for AT42QT2120
    
    void _InitQT1070(void);
    void _InitQT2120(void);
    uint8_t _ReadSingleReg(bool isQTouch2120, uint8_t reg);
    void _WriteSingleReg(bool isQTouch2120, uint8_t reg, uint8_t value);  
    
  public:
    QTouchBoard(int int1070, int int2120);
    void begin(TwoWire &inStream);
    ~QTouchBoard();
    void initQTouch(void);
    bool isValueUpdate(void);
    uint8_t QT2120ReadSingleReg(uint8_t reg);
    uint8_t QT1070ReadSingleReg(uint8_t reg);
    void QT1070WriteSingleReg(uint8_t reg, uint8_t value);
    void QT2120WriteSingleReg(uint8_t reg, uint8_t value);
};

#endif  // __QTOUCHBOARD_H__
