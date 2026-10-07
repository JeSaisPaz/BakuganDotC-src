// bdc 0x08a1075c GmoImageHeapRelease
#include "bdc.h"

/* Drops a reference to the block holding `ptr` (`GmoImageBlockRelease`), searching pool `pool` of
   the image library's 3-pool block heap (pool records of 0x24 bytes at `0x08af1258`: allocator,
   free function, alignment, flags, block list, lookup cache, address range) first and then the
   other pools. Returns `ptr`. */

void *GmoImageHeapRelease(int pool, void *ptr)

{
  u32 *block;
  int tries;
  
  if (ptr != (void *)0x0) {
    tries = 0;
    do {
      block = GmoImageHeapFindBlock(pool,ptr);
      tries = tries + 1;
      if (block != (u32 *)0x0) {
        GmoImageBlockRelease(block);
        return ptr;
      }
      pool = (pool + 1) % 3;
    } while (tries != 3);
  }
  return ptr;
}

