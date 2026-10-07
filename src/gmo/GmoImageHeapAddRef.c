// bdc 0x08a0ff74 GmoImageHeapAddRef
#include "bdc.h"

/* Adds a reference to the block holding `ptr`, searching pool `pool` of the image library's 3-pool
   block heap (pool records of 0x24 bytes at `0x08af1258`: allocator, free function, alignment,
   flags, block list, lookup cache, address range) first and then the other two pools. Returns
   `ptr`. */

void *GmoImageHeapAddRef(int pool, void *ptr)
{
  u32 *block;
  int tries;

  if (ptr != NULL) {
    tries = 0;
    do {
      block = GmoImageHeapFindBlock(pool, ptr);
      tries = tries + 1;
      if (block != NULL) {
        GmoImageBlockAddRef(block);
        return ptr;
      }
      pool = (pool + 1) % 3;
    } while (tries != 3);
  }
  return ptr;
}
