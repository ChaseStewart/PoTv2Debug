
#ifndef __QTOUCH_H__
#define __QTOUCH_H__

#define QTOUCH2120_ADDR  0x1C
#define QTOUCH1070_ADDR  0x1B
#define OFST_QT1070_INTEGRATION_REG  46
#define OFST_QT1070_AVE_AKS_REG  39


/* QT2120 commands */
int QT2120ReadSingleReg(bool isFretBoard, uint8_t reg);
int QT2120WriteSingleReg(bool isFretBoard, uint8_t reg, uint8_t value);
void QT2120Init(bool isFretBoard);

/* QT1070 commands */
int QT1070ReadSingleReg(bool isFretBoard,uint8_t reg);
int QT1070WriteSingleReg(bool isFretBoard,uint8_t reg, uint8_t value);
void QT1070Init(bool isFretBoard);

#endif  // __QTOUCH_H__
