// bdc 0x089bfddc SndEmitterSystemCreate
#include "bdc.h"

/* Creates the positional-sound runtime. Every object is allocated from the low end of the heap
   under `MemLock` (the allocation mode is switched to "low" and restored afterwards): the
   0x28-byte `SndListener` (constructed by `SndListenerInit`, stored in `g_soundListener`),
   the priority list `g_soundEmitterList` (`SndEmitterListInit`, capacity 0x40), the `MemPool`
   `g_soundEmitterPool` that supplies `SndEmitter` records (`MemPoolInit(pool, 0x48, 0x40, 1)`)
   and the scratch list `g_soundEmitterScratchList` (capacity 0x80, used by
   `SndEmitterAssignGroups`). A failed allocation leaves its global NULL. */

void SndEmitterSystemCreate(void)
{
  bool fromLow;
  SndListener *listener;
  CorePrioList *list;
  MemPool *pool;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  listener = MemAlloc(0x28, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (listener != NULL) {
    SndListenerInit(listener);
  }
  g_soundListener = listener;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  list = MemAlloc(0x14, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (list != NULL) {
    SndEmitterListInit(list, 0x40);
  }
  g_soundEmitterList = list;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  pool = MemAlloc(0x14, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (pool != NULL) {
    MemPoolInit(pool, 0x48, 0x40, true);
  }
  g_soundEmitterPool = pool;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  list = MemAlloc(0x14, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (list != NULL) {
    SndEmitterListInit(list, 0x80);
  }
  g_soundEmitterScratchList = list;
}
