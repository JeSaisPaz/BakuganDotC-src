// bdc 0x088f51fc GameEventFlagRangeIndex
#include "bdc.h"

/* Returns `id` minus the first flag of class `kind` (`g_gameEventFlagClassBase[kind]`): the index of
   the flag inside its class. Callers pass `kind` 1, 2, 3, 5 or 6 only (domain 0..7, unguarded). */

s16 GameEventFlagRangeIndex(s32 kind, s16 id)

{
  return id - g_gameEventFlagClassBase[kind];
}

