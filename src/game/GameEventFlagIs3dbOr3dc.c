// bdc 0x088f518c GameEventFlagIs3dbOr3dc
#include "bdc.h"

/* True for story flags 0x3db and 0x3dc (special-cased by `GameEventFlagSet`). */

s32 GameEventFlagIs3dbOr3dc(s16 id)

{
  if ((id != 0x3db) && (id != 0x3dc)) {
    return 0;
  }
  return 1;
}

