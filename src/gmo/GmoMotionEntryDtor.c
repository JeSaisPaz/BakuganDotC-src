// bdc 0x089da450 GmoMotionEntryDtor
#include "bdc.h"

/* Virtual destructor of `GmoMotionEntry` (vtable `g_gmoMotionEntryVtable` entry 1): frees the arena block
   (`+0x80`) under `MemLock`, switches to the base vtable `g_gmoMotionBaseVtbl`, runs `CoreNodeDtor` and
   frees the entry when bit 0 of `flags` is set (`GmoMotionFree` passes 3 for stand-alone entries
   and 2 for array elements). */

void GmoMotionEntryDtor(GmoMotionEntry *entry, u32 flags)

{
  u8 *ptr;
  
  if (entry != (GmoMotionEntry *)0x0) {
    ptr = entry->arena;
    ((CoreNode *)entry)->vtable = g_gmoMotionEntryVtable;
    if (ptr != (u8 *)0x0) {
      MemLock();
      MemFree(ptr,(char *)0x0,0);
      MemUnlock();
      entry->arena = (u8 *)0x0;
    }
    ((CoreNode *)entry)->vtable = g_gmoMotionBaseVtbl;
    CoreNodeDtor((CoreNode *)entry,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(entry,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

