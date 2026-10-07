// bdc 0x089c1c1c SndObjectSlotPoolCreate
#include "bdc.h"

/* (Re)builds the slot pool `g_soundObjectSlotPool`: an existing pool is destroyed with
   `MemPoolDestroy` (flags 3, header freed) and a new 0x14-byte `MemPool` header is allocated
   from the low heap and initialised with `MemPoolInit(pool, 4, count, 1)`, i.e. `count` items of 4
   bytes. `SndObjectInit` takes the emitter-pointer arrays of the sound objects from it (several
   consecutive items per object). */

void SndObjectSlotPoolCreate(s32 count)

{
  bool fromLow;
  MemPool *pool;
  
  if (g_soundObjectSlotPool != (MemPool *)0x0) {
    MemPoolDestroy(g_soundObjectSlotPool,3);
    g_soundObjectSlotPool = (MemPool *)0x0;
  }
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  pool = MemAlloc(0x14,(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (pool != (MemPool *)0x0) {
    MemPoolInit(pool,4,count,true);
  }
  g_soundObjectSlotPool = pool;
  return;
}

