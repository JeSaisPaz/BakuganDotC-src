// bdc 0x08a32300 GmoMotionEntryGetArena
#include "bdc.h"

/* `GmoMotionEntry` virtual method (vtable `0x08af53c4` entry 8, get arena): returns the track
   data arena pointer (`+0x80`). */

u8 *GmoMotionEntryGetArena(GmoMotionEntry *entry)

{
  return entry->arena;
}

