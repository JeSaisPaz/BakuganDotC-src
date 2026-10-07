// bdc 0x089f6974 GfxTexFreeMain
#include "bdc.h"

/* Free callback matching `GfxTexAllocMain`: `MemFree` under `MemLock`. */

void GfxTexFreeMain(void *ptr)

{
  MemLock();
  MemFree(ptr,(char *)0x0,0);
  MemUnlock();
  return;
}

