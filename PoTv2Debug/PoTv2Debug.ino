#include <Wire.h>
#include "QTouch.h"
#include "FretBoard.h"
#include "StrumBoard.h"

uint8_t fret; 
uint8_t prevFret;
uint8_t keyStatus0;
uint8_t keyStatus1;
uint8_t keyStatus2;

uint16_t key;
uint16_t prevKey;
uint8_t strumStatus0;
uint8_t strumStatus1;
uint8_t strumStatus2;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  Serial.println("*** Paddle of Theseus Fretboard Test v2 ***");
  Serial.println();

  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(14, INPUT);
  pinMode(15, INPUT); 
   
  Wire.begin();
  Wire1.begin();
  delay(2000);
  fret = 0;

  
  Serial.println("*** Setup FretBoard ***");
  Serial.print("Attempting to contact ATQT2120... ");
  QT2120Init(true);

  Serial.print("Attempting to contact ATQT1070... ");
  QT1070Init(true);
  Serial.println("*** Done ***");

  Serial.println("*** Setup StrumBoard ***");
  Serial.print("Attempting to contact ATQT2120... ");
  QT2120Init(false);

  Serial.print("Attempting to contact ATQT1070... ");
  QT1070Init(false);
  Serial.println("*** Done ***");
  
  Serial.println("Done!");
}

void loop()
{
  if (!digitalRead(14) || !digitalRead(15))
  {
    keyStatus0 = QT2120ReadSingleReg(true, 3);
    keyStatus1 = QT2120ReadSingleReg(true, 4);
    keyStatus2 = QT1070ReadSingleReg(true, 3);
    fret = GetFret(keyStatus0, keyStatus1, keyStatus2);
    if (prevFret != fret)
    {
      Serial.print("Fret=");    Serial.print(fret); 
      Serial.print(", ks0=0x"); Serial.print(keyStatus0, HEX); 
      Serial.print(", ks1=0x"); Serial.print(keyStatus1, HEX);
      Serial.print(", ks2=0x"); Serial.println(keyStatus2, HEX);
    }
    prevFret = fret;
  }

  if (!digitalRead(1) || !digitalRead(0))
  {
    strumStatus0 = QT2120ReadSingleReg(false, 3);
    strumStatus1 = QT2120ReadSingleReg(false, 4);
    strumStatus2 = QT1070ReadSingleReg(false, 3);
    key = GetStrumKey(strumStatus0, strumStatus1, strumStatus2);
    if (prevKey != key)
    {
      Serial.print("Strum=");    Serial.print(key, HEX); 
      Serial.print(", ss0=0x"); Serial.print(strumStatus0, HEX); 
      Serial.print(", ss1=0x"); Serial.print(strumStatus1, HEX);
      Serial.print(", ss2=0x"); Serial.println(strumStatus2, HEX);
    }
    prevKey = key;
  }
}
