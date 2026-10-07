// bdc 0x0892ad8c UiBakuganListOrder
#include "bdc.h"

/* Converts between Bakugan ids and positions in the select-screen grid: with `toId == 0` returns
   the position of id `n` (table `0x08ac1994`, 21 bytes), otherwise the id at position `n` (table
   `0x08ac19a9`, 20 bytes). */

u8 UiBakuganListOrder(void *screen, bool toId, u32 n)

{
  u8 result;
  u8 posTable [24];
  u8 idTable [20];
  
  memcpy(posTable,g_bakuganListOrderPos,0x15);
  memcpy(idTable,g_bakuganListOrderId,0x14);
  if (toId) {
    result = idTable[n & 0xff];
  }
  else {
    result = posTable[n & 0xff];
  }
  return result;
}

