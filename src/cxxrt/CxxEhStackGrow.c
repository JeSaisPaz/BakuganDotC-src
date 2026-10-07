// bdc 0x08a0359c CxxEhStackGrow
#include "bdc.h"

/* Adds a heap region for the exception-object stack big enough for `size` (rounded to 8 KiB, plus 8
   KiB when less than 4 KiB would remain): the region descriptor is itself carved from the current
   region, the buffer comes from `CxxEhMalloc`, and it becomes the current region. */

void CxxEhStackGrow(u32 size)

{
  CxxEhBlock *blk;
  CxxEhRegion *region;
  void *buf;
  u32 size_00;

  size_00 = (size & 0xffffe000) + 0x2000;
  if (size_00 - size < 0x1000) {
    size_00 = (size & 0xffffe000) + 0x4000;
  }
  CxxEhStackPush(0x20, &blk);
  blk->flag = 1;
  region = (CxxEhRegion *)blk->end;
  buf = CxxEhMalloc(size_00);
  region->next = g_cxxEhAllocRegion;
  g_cxxEhAllocRegion = region;
  region->base = buf;
  region->size = size_00;
  region->used = 0;
  region->heap = 1;
}
