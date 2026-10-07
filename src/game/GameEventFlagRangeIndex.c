// bdc 0x088f51fc GameEventFlagRangeIndex
#include "bdc.h"

/* Returns `id` minus the first flag of class `kind` (`0x08a99800[kind]`): the index of the flag
   inside its class. */

s16 GameEventFlagRangeIndex(s32 kind, s16 id)

{
  return id - g_gameEventFlagClassBase[kind];
}

