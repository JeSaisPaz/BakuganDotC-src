// bdc 0x088f51b4 GameEventFlagIsSpecialSet394
#include "bdc.h"

/* True for story flags 0x394 and 0x3a0..0x3a4 (special-cased by `GameEventFlagSet`). */

s32 GameEventFlagIsSpecialSet394(s16 id)

{
  if ((((id != 0x3a0) && (id != 0x394)) && (id != 0x3a1)) &&
     (((id != 0x3a2 && (id != 0x3a3)) && (id != 0x3a4)))) {
    return 0;
  }
  return 1;
}

