// bdc 0x089de768 GfxModelAllocWork
#include "bdc.h"

/* Allocates the model's GMO work area from the low heap and makes it the GMO bump region
   (`GmoSetBumpRegion`, offset word `+0x124`): with `size` 0 the default 0x200f-byte buffer at
   `+0x118` (0x2000 usable), otherwise `size` bytes at `+0x11c` (size in `+0x120`). */

void GfxModelAllocWork(GfxModel *self, u32 size)

{
  bool wasLow;
  void *buf;
  
  self->bumpUsed = 0;
  if (size == 0) {
    self->work = (void *)0x0;
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    buf = MemAlloc(0x200f,(char *)0x0,0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    self->workDefault = buf;
    GmoSetBumpRegion(buf,&self->bumpUsed,0x2000);
  }
  else {
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    buf = MemAlloc(size,(char *)0x0,0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    self->work = buf;
    self->workSize = size;
    GmoSetBumpRegion(buf,&self->bumpUsed,size);
  }
  return;
}

