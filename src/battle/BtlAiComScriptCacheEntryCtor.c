// bdc 0x0889b18c BtlAiComScriptCacheEntryCtor
#include "bdc.h"

/* Element constructor of the 0x10-byte `BtlAiScriptCacheEntry` records of the AI rule-script
   cache (`BtlAiComScriptCacheAcquire`): clears the refcount, the header bytes `+2..+6`, the
   group count and the group table; returns `entry`. Keeps the `void *` signature of a
   `CxxVecNewSimple` element constructor. */

void *BtlAiComScriptCacheEntryCtor(void *entry)
{
  BtlAiScriptCacheEntry *rec = (BtlAiScriptCacheEntry *)entry;

  rec->refCount = 0;
  rec->header[3] = 0;
  rec->header[2] = 0;
  rec->header[1] = 0;
  rec->header[0] = 0;
  rec->header[4] = 0;
  rec->groupCount = 0;
  rec->groups = NULL;
  return entry;
}
