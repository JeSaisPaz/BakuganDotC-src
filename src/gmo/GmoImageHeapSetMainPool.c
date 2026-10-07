// bdc 0x08a0fc74 GmoImageHeapSetMainPool
#include "bdc.h"

/* Configures pool 0 (main memory) of the image library's 3-pool block heap (pool records of 0x24
   bytes at `0x08af1258`: allocator, free function, alignment, flags, block list, lookup cache,
   address range) (`GmoImageHeapSetPool`) and lets pool 1 inherit it when it has no allocator of
   its own. */

void GmoImageHeapSetMainPool(void *alloc, void *free, u16 align, u8 flag)

{
  GmoImageHeapSetPool(0,alloc,free,align,flag);
  GmoImageHeapInheritPool(1,0);
  return;
}

