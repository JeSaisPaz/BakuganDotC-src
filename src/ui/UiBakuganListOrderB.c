// bdc 0x0892b0d4 UiBakuganListOrderB
#include "bdc.h"

/* Second grid-order table pair of `UiBakuganListOrder`: `toId == 0` → table `0x08ac19bd`,
   otherwise `0x08ac19d2`. */

u8 UiBakuganListOrderB(void *screen, bool toId, u32 n)

{
  u8 result;
  u8 posTable [24];
  u8 idTable [20];
  
  memcpy(posTable,g_bakuganListOrderBPos,0x15);
  memcpy(idTable,g_bakuganListOrderBId,0x14);
  if (toId) {
    result = idTable[n & 0xff];
  }
  else {
    result = posTable[n & 0xff];
  }
  return result;
}

