// bdc 0x08a322e0 GmoMotionEntryNameEquals
#include "bdc.h"

/* `GmoMotionEntry` virtual method (vtable `0x08af53c4` entry 5, name equals): case-insensitive
   comparison (`strcasecmp`) of the entry's name with `name`; true when equal. */

bool GmoMotionEntryNameEquals(GmoMotionEntry *entry, const char *name)

{
  int cmp;
  
  cmp = strcasecmp(entry->name,name);
  return cmp == 0;
}

