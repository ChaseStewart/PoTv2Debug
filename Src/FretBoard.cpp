#include "FretBoard.h"

uint8_t GetFret(uint8_t ks0, uint8_t ks1, uint8_t ks2)
{
  if (ks2)
  {
    if (ks2 & 0x40) return 19;
    else if (ks2 & 0x20) return 18;
    else if (ks2 & 0x10) return 17;
    else if (ks2 & 0x08) return 16;
    else if (ks2 & 0x04) return 15;
    else if (ks2 & 0x02) return 14;
    else if (ks2 & 0x01) return 13;
  }
  if (ks1)
  {
    if      (ks1 & 0x08) return 12;
    else if (ks1 & 0x04) return 11;
    else if (ks1 & 0x02) return 10;
    else if (ks1 & 0x01) return 9;
  }
  if (ks0)
  {
    if (ks0 & 0x80) return 8;
    else if (ks0 & 0x40) return 7;
    else if (ks0 & 0x20) return 6;
    else if (ks0 & 0x10) return 5;
    else if (ks0 & 0x08) return 4;
    else if (ks0 & 0x04) return 3;
    else if (ks0 & 0x02) return 2;
    else if (ks0 & 0x01) return 1;
    else return 0;
  }
  return 0;
}
