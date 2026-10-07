// bdc 0x08a0353c CxxEhStackPush
#include "bdc.h"

/* Carves a block of `size` bytes plus a 0x10-byte header from the current exception-memory region
   (`g_cxxEhAllocRegion`): links it on the allocation stack (`g_cxxEhStackTop`), records its
   size, stores the block header pointer in `*out`. */

void CxxEhStackPush(int size, CxxEhBlock **out)

{
  CxxEhRegion *region = (CxxEhRegion *)g_cxxEhAllocRegion;
  u32 used = region->used;
  u32 data = used + 0x10;

  *out = (CxxEhBlock *)((u8 *)region->base + used);
  region = (CxxEhRegion *)g_cxxEhAllocRegion;
  {
    u8 *base = (u8 *)region->base;
    region->used = data + size;
    (*out)->next = g_cxxEhStackTop;
    (*out)->end = base + data;
  }
  g_cxxEhStackTop = *out;
  g_cxxEhStackTop->size = size;
  (*out)->flag = 0;
}
