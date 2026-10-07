// bdc 0x08a322bc GmoMotionEntrySetName
#include "bdc.h"

/* `GmoMotionEntry` virtual method (vtable `0x08af53c4` entry 3, set name): `strcpy`s `name` into
   the entry's name buffer (`+0x5c`, 0x20 bytes, no bounds check). */

void GmoMotionEntrySetName(GmoMotionEntry *entry, const char *name)

{
  strcpy(entry->name,name);
  return;
}

