#include <Wire.h>
#include "QTouch.h"

int QT2120ReadSingleReg(bool isFretBoard, uint8_t reg)
{
  TwoWire selectStream = (isFretBoard) ? Wire: Wire1;
  
  selectStream.beginTransmission(QTOUCH2120_ADDR);
  selectStream.write(reg);
  selectStream.endTransmission(false); //endTransmission but keep the connection active

  selectStream.requestFrom(QTOUCH2120_ADDR, 1); //Ask for 1 byte, once done, bus is released by default

  while(!selectStream.available()) ; //Wait for the data to come back
  return selectStream.read(); //Return this one byte
}

int QT2120WriteSingleReg(bool isFretBoard, uint8_t reg, uint8_t value)
{
  TwoWire selectStream = (isFretBoard) ? Wire: Wire1;

  selectStream.beginTransmission(QTOUCH2120_ADDR);
  selectStream.write(reg);
  selectStream.write(value);
  selectStream.endTransmission(); //Stop transmitting  
}

void QT2120Init(bool isFretBoard)
{
  /* Get Chip ID */
  uint8_t chipId = QT2120ReadSingleReg(isFretBoard, 0);
  Serial.print("QT2120 chipId = 0x"); Serial.print(chipId, HEX); Serial.println(", should be 0x3E");

  /* Get FW version */
  uint8_t versionByte = QT2120ReadSingleReg(isFretBoard, 1);
  uint8_t versionMajor = (versionByte & 0xF0) >> 4;
  uint8_t versionMinor = (versionByte & 0x0F);
  Serial.print("Firmware version = "); Serial.print(versionMajor); Serial.print("."); Serial.println(versionMinor);

  /* Set touch integration */
  QT2120WriteSingleReg(isFretBoard, 11, 4);

  /* Set drift hold time */
  QT2120WriteSingleReg(isFretBoard, 13, 3);

  /* Detect Threshold */
  for (int i=0; i<12; i++)
  {
      QT2120WriteSingleReg(isFretBoard, 16+i, 19);
  } 
}

int QT1070ReadSingleReg(bool isFretBoard, uint8_t reg)
{
  TwoWire selectStream = (isFretBoard) ? Wire: Wire1;

  selectStream.beginTransmission(QTOUCH1070_ADDR);
  selectStream.write(reg);
  selectStream.endTransmission(false); //endTransmission but keep the connection active

  selectStream.requestFrom(QTOUCH1070_ADDR, 1); //Ask for 1 byte, once done, bus is released by default

  while(!selectStream.available()) ; //Wait for the data to come back
  return selectStream.read(); //Return this one byte
}

int QT1070WriteSingleReg(bool isFretBoard, uint8_t reg, uint8_t value)
{
  TwoWire selectStream = (isFretBoard) ? Wire: Wire1;

  selectStream.beginTransmission(QTOUCH1070_ADDR);
  selectStream.write(reg);
  selectStream.write(value);
  selectStream.endTransmission(); //Stop transmitting  
}

void QT1070Init(bool isFretBoard)
{
  /* Get Chip ID */
  uint8_t chipId = QT1070ReadSingleReg(isFretBoard, 0);
  Serial.print("QT1070 chipId = 0x"); Serial.print(chipId, HEX); Serial.println(", should be 0x2E");

  /* Get FW version */
  uint8_t versionByte = QT1070ReadSingleReg(isFretBoard, 1);
  uint8_t versionMajor = (versionByte & 0xF0) >> 4;
  uint8_t versionMinor = (versionByte & 0x0F);
  Serial.print("Firmware version = "); Serial.print(versionMajor); Serial.print("."); Serial.println(versionMinor);

  /* Set touch integration */
  for (int i=0; i<7; i++)
  {
    QT1070WriteSingleReg(isFretBoard, i + OFST_QT1070_INTEGRATION_REG, 4);
    QT1070WriteSingleReg(isFretBoard, i + OFST_QT1070_AVE_AKS_REG, 0x20);
  }

  /* Set Low Power Mode */
  QT1070WriteSingleReg(isFretBoard, 54, 1);
}
