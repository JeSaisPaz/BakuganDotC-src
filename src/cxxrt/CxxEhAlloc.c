// bdc 0x08a03638 CxxEhAlloc
#include "bdc.h"

/* Allocates `size` bytes (rounded up to 16) on the exception-object stack, initialising it on
   first use (`CxxEhStackInit`) and growing it when the current region lacks room for the
   block plus 0x40 bytes (`CxxEhStackGrow`). Returns the data pointer of the block carved by
   `CxxEhStackPush` (the block's `end` field, header + 0x10). */

void *CxxEhAlloc(u32 size)

{
  CxxEhRegion *region;
  CxxEhBlock *block;
  u32 pad;

  if (g_cxxEhAllocRegion == NULL) {
    CxxEhStackInit();
  }
  pad = 0;
  if ((size & 0xf) != 0) {
    pad = 0x10 - (size & 0xf);
  }
  size += pad;
  region = (CxxEhRegion *)g_cxxEhAllocRegion;
  if (region->size < size + region->used + 0x40) {
    CxxEhStackGrow(size);
  }
  CxxEhStackPush(size, &block);
  /* The asm returns CxxEhStackPush's leftover v0, which is the value it stored in block->end. */
  return block->end;
}
