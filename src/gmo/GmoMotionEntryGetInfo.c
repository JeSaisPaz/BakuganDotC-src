// bdc 0x08a322b4 GmoMotionEntryGetInfo
#include "bdc.h"

/* `GmoMotionEntry` virtual method (vtable `0x08af53c4` entry 2, get info): returns the
   `GmoMotionInfo` data block, `entry + 0x2c`. */

GmoMotionInfo *GmoMotionEntryGetInfo(GmoMotionEntry *entry)

{
  return &entry->info;
}

