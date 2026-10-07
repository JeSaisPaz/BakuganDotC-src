// bdc 0x089c483c SndDecOutGet
#include "bdc.h"

/* Returns the `SndDecOut` stored in slot `channel` of `g_soundDecOutTable` (NULL if free). No
   bounds or table-NULL check: callers test `SndDecOutExists` first (the thread entries rely on
   the table existing). */

SndDecOut *SndDecOutGet(s32 channel)

{
  return g_soundDecOutTable->slots[channel];
}

