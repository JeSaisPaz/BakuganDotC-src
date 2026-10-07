// bdc 0x08a322d8 GmoMotionEntryGetName
#include "bdc.h"

/* `GmoMotionEntry` virtual method (vtable `0x08af53c4` entry 4, get name): returns the name
   buffer, `entry + 0x5c`. */

const char *GmoMotionEntryGetName(GmoMotionEntry *entry)

{
  return entry->name;
}

