#include "StrumBoard.h"

uint8_t GetStrumKey(uint8_t ss0, uint8_t ss1, uint8_t ss2)
{
  uint8_t result = 0;
  if (ss2 >> 3)
  {
    result |= 0x1;
  }
  if (((ss0 >> 5) & 0x0F) || (ss1 & 0x01))
  {
    result |= 0x2;
  } 
  if ((ss0 >> 1) & 0x0F)
  {
    result |= 0x4;
  } 
  if (((ss1 >> 1) & 0x0E) || (ss0 & 0x01))
  {
    result |= 0x8;
  } 

  
  return result;
  
}
