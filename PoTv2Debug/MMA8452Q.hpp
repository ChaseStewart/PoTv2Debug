/*!
 * \file MMA8452Q.hpp
 *
 * \brief Header for 6DOF IMU class used to get isLefty
 *
 * Author: Chase E. Stewart for Hidden Layer Design
 *
 */
#ifndef __MMA8452Q_HPP__
#define __MMA8452Q_HPP__
 
#include "Arduino.h"

// MMA8452Q Slave Addr and simple WHOAMI response
#define MMA8452Q_SLAVE_ADDR     0x1D  ///< Address for contacting MMA8452Q
#define MMA8452Q_WHOAMI_REG     0x0D  ///< Register WHOAMI for conducting sanity checks
#define MMA8452Q_WHOAMI_VAL     0x2A  ///< Expected value from datasheet for reading WHOAMI register

// MMA8452Q Registers
#define MMA8452Q_CTRL1_REG        0x2A  ///< Address for CTRL 1 register
#define MMA8452Q_XYZ_DATA_CFG_REG 0x0E  ///< Address for XYZ_DATA_CFG register
#define MMA8452Q_OUT_X_MSB_REG    0x01  ///< Address for MSB of multi-byte X-axis output
                                        ///< NOTE: a continuous read is done starting from this address to read the rest of the registers

#define GSCALE 2  ///< Sets full-scale range to +/-2, 4, or 8g. Used to calc real g values.


/**************************************************************************/
/*!
    \brief  Class for interacting with the MMA8452Q IMU
    This class provides just what is needed to get isLefty for the paddle
*/
/**************************************************************************/
class MMA8452Q
{
  private:
    int8_t MMA8452QSetRegisters(void); 
    uint8_t _slave_addr;  ///< I2C Address for this MMA8452Q device
    
  public:
    MMA8452Q(void);
    int8_t init(void);
    void Update(void);
    void PrintAccel(void);
    bool IsLeftyFlipped(void);
    float x;  ///< X value from IMU
    float y;  ///< Y value from IMU
    float z;  ///< Z value from IMU
};

#endif /* __MMA8452Q_HPP__ */
