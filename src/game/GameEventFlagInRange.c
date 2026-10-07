// bdc 0x088f4fd0 GameEventFlagInRange
#include "bdc.h"

/* True when flag `id` lies in flag class `kind` (`0x08a99800[kind] <= id <= 0x08a99810[kind]`).
   Used by `GameEventFlagTest`/`GameEventFlagSet`/`GameEventFlagClear` and the event opcodes.
    */

s32 GameEventFlagInRange(s32 kind, u16 id)

{
  if ((g_gameEventFlagClassBase[kind] <= id) && (id <= g_gameEventFlagClassLimit[kind])) {
    return 1;
  }
  return 0;
}

