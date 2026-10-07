// bdc 0x089f68ac GfxTexAllocMain
#include "bdc.h"

/* Allocation callback registered by `GfxTextureSystemReset` for the model library's main-memory
   pool: `MemAlloc``(size)` from the low end of the game heap under `MemLock`. */

void *GfxTexAllocMain(s32 size)

{
  bool fromLow;
  void *mem;
  
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(size,(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  return mem;
}

