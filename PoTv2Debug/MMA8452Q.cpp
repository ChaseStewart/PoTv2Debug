/*!
 * \file MMA8452Q.cpp
 *
 * \brief IMU class used to get isLefty, originally written by SparkFun
 *
 * NOTE: this code has been edited from the below, original Sparkfun header starts from
 * second set of asterisks!
 *
 * Author: Chase E. Stewart for Hidden Layer Design
 *
 */

/******************************************************************************
SparkFun_MMA8452Q.cpp
SparkFun_MMA8452Q Library Source File
Jim Lindblom \ SparkFun Electronics
Original Creation Date: June 3, 2014
https://github.com/sparkfun/MMA8452_Accelerometer
This file implements all functions of the MMA8452Q class. Functions here range
from higher level stuff, like reading/writing MMA8452Q registers to low-level,
hardware I2C reads and writes.
Development environment specifics:
  IDE: Arduino 1.0.5
  Hardware Platform: Arduino Uno
  **Updated for Arduino 1.6.4 5/2015**
  
This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!
Distributed as-is; no warranty is given.
******************************************************************************/

#include "Arduino.h"
#include "MMA8452Q.hpp"
#include <Wire.h>

static byte readRegister(byte addressToRead);
static void readRegisters(byte addressToRead, int bytesToRead, byte * dest);
static void writeRegister(byte addressToWrite, byte dataToWrite);
static void MMA8452Standby(void);
static void MMA8452Active(void);


/**************************************************************************/
/*!
    \brief  Instantiate MMA8452Q class and set slave address
*/
/**************************************************************************/
MMA8452Q::MMA8452Q(void)
{
  _slave_addr = MMA8452Q_SLAVE_ADDR;
  x = 0;
  y = 0;
  z = 0;
}

/**************************************************************************/
/*!
    \brief  Test and set up the IMU
    \return 0 == failure, else 1
*/
/**************************************************************************/
int8_t MMA8452Q::init(void)
{
  byte c = readRegister(MMA8452Q_WHOAMI_REG);  // Read WHOAMI register
  if (c == MMA8452Q_WHOAMI_VAL) // WHOAMI should always be 0x2A
  {  
    Serial.println("INFO: MMA8452Q is online...");
  }
  else
  {
    Serial.print("WARNING: Could not connect to MMA8452Q: 0x");
    Serial.println(c, HEX);
    Serial.println("WARNING: Failed to init accelerometer!");  
    return 0;
  }
  
  MMA8452Standby();  // Must be in standby to change registers

  byte fsr = GSCALE;  // Set up the full scale range to 2, 4, or 8g.
  if(fsr > 8) fsr = 8;  //Easy error check
  fsr >>= 2;  // Neat trick, see page 22. 00 = 2G, 01 = 4A, 10 = 8G
  writeRegister(MMA8452Q_XYZ_DATA_CFG_REG, fsr);

  // The default data rate is 800Hz and we don't modify it in this example code

  MMA8452Active();  // Set to active to start reading
  return 1;
}

/**************************************************************************/
/*! 
    \brief  Update the accelerometer values, store them in class vars x,y,z 
 */
/**************************************************************************/
void MMA8452Q::Update(void)
{
  byte rawData[6];  // x/y/z accel register data stored here

  readRegisters(MMA8452Q_OUT_X_MSB_REG, 6, rawData);  // Read the six raw data registers into data array
  
  x = ((short)(rawData[0]<<8 | rawData[1])) >> 4;
  y = ((short)(rawData[2]<<8 | rawData[3])) >> 4;
  z = ((short)(rawData[4]<<8 | rawData[5])) >> 4;
}


/**************************************************************************/
/*!
    \brief  Print current IMU x,y,z as a float
*/
/**************************************************************************/
void MMA8452Q::PrintAccel(void)
{  
    Serial.print("\rINFO: X_ACCEL: ");
    Serial.print((float) x, 4);
    Serial.print(", Y_ACCEL: ");
    Serial.print((float) y, 4);
    Serial.print(", Z_ACCEL: ");
    Serial.print((float) z, 4);
}

/**************************************************************************/
/*!
    \brief  Return whether paddle is physically lefty-flipped
    accelerometer value of (x < 0) implies a left-handed person
    is using the instrument
    
    \return bool True == The paddle is lefty flipped, else False
*/
/**************************************************************************/
bool MMA8452Q::IsLeftyFlipped(void)
{
  return (x < 0);
}


/**************************************************************************/
/*!
    \brief  Read a single byte from addressToRead and return it as a byte
    \param  addressToRead 
            The address of the register to read
    \return byte Value returned from register
*/
/**************************************************************************/
static byte readRegister(byte addressToRead)
{
  Wire1.beginTransmission(MMA8452Q_SLAVE_ADDR);
  Wire1.write(addressToRead);
  Wire1.endTransmission(false); //endTransmission but keep the connection active

  Wire1.requestFrom(MMA8452Q_SLAVE_ADDR, 1); //Ask for 1 byte, once done, bus is released by default

  while(!Wire1.available()) ; //Wait for the data to come back
  return Wire1.read(); //Return this one byte
}

/**************************************************************************/
/*!
    \brief  Read bytesToRead sequentially, starting at addressToRead into the dest byte array
    \param  addressToRead 
            the address of the register to read
    \param  bytesToRead 
            count of bytes to read
    \param  dest 
            pointer to the destination buffer for data
*/
/**************************************************************************/
static void readRegisters(byte addressToRead, int bytesToRead, byte * dest)
{
  Wire1.beginTransmission(MMA8452Q_SLAVE_ADDR);
  Wire1.write(addressToRead);
  Wire1.endTransmission(false); //endTransmission but keep the connection active

  Wire1.requestFrom(MMA8452Q_SLAVE_ADDR, bytesToRead); //Ask for bytes, once done, bus is released by default

  while(Wire1.available() < bytesToRead); //Hang out until we get the # of bytes we expect

  for(int x = 0 ; x < bytesToRead ; x++)
  {
    dest[x] = Wire1.read();
  }    
}

/**************************************************************************/
/*!
    \brief  Writes a single byte (dataToWrite) into addressToWrite
    \param  addressToWrite
            the address of the register to be written into
    \param  dataToWrite 
            byte of data to be written
*/
/**************************************************************************/
static void writeRegister(byte addressToWrite, byte dataToWrite)
{
  Wire1.beginTransmission(MMA8452Q_SLAVE_ADDR);
  Wire1.write(addressToWrite);
  Wire1.write(dataToWrite);
  Wire1.endTransmission(); //Stop transmitting
}

/**************************************************************************/
/*!
    \brief  set MMA8452Q into standby mode
    The chip must be in standby to change most register settings
*/
/**************************************************************************/
static void MMA8452Standby(void)
{
  byte c = readRegister(MMA8452Q_CTRL1_REG);
  writeRegister(MMA8452Q_CTRL1_REG, c & ~(0x01)); //Clear the active bit to go into standby
}

/**************************************************************************/
/*!
    \brief  set MMA8452Q into active mode
    The chip must be in active mode to update and output data
*/
/**************************************************************************/
static void MMA8452Active(void)
{
  byte c = readRegister(MMA8452Q_CTRL1_REG);
  writeRegister(MMA8452Q_CTRL1_REG, c | 0x01); //Set the active bit to begin detection
}
