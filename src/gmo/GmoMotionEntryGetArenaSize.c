// bdc 0x08a32308 GmoMotionEntryGetArenaSize
#include "bdc.h"

/* `GmoMotionEntry` virtual method (vtable `0x08af53c4` entry 9, get arena size): returns the
   reserved arena size (`+0x7c`). */

s32 GmoMotionEntryGetArenaSize(GmoMotionEntry *entry)

{
  return entry->arenaSize;
}

