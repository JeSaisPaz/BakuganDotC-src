// bdc 0x08a32310 GmoMotionEntrySetArena
#include "bdc.h"

/* `GmoMotionEntry` virtual method (vtable `0x08af53c4` entry 10, set arena): sets the track data
   arena pointer (`+0x80`). */

void GmoMotionEntrySetArena(GmoMotionEntry *entry, u8 *arena)

{
  entry->arena = arena;
  return;
}

