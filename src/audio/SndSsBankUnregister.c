// bdc 0x08a1ff34 SndSsBankUnregister
#include "bdc.h"

/* Removes bank `bankId` from the Sony sound layer's 0x80-entry bank table (`g_sndSsBankTable[bankId] =
   NULL`). Returns 0, `0x80450002` for an out-of-range or empty slot. Returns `0x80450001` when the
   layer is not initialised. */

s32 SndSsBankUnregister(u32 bankId)

{
  s32 result;
  
  result = -0x7fbaffff;
  if (g_sndSsState != -1) {
    result = -0x7fbafffe;
    if ((bankId < 0x80) && (result = -0x7fbafffe, g_sndSsBankTable[bankId] != 0)) {
      g_sndSsBankTable[bankId] = 0;
      result = 0;
    }
  }
  return result;
}

