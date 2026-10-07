// bdc 0x08a0fe64 GmoImageHeapFindBlock
#include "bdc.h"

/* Finds the block of pool `pool` of the image library's 3-pool block heap (pool records of 0x24
   bytes at `0x08af1258`: allocator, free function, alignment, flags, block list, lookup cache,
   address range) that contains `ptr`: checks the two cached blocks, the pool's address range, then
   walks the block list; updates the cache. Returns the block trailer or NULL. */

u32 *GmoImageHeapFindBlock(int pool, u32 *ptr)
{
  GmoImagePool *p = &g_gmoImagePools[pool];
  GmoImageBlock *c0;
  GmoImageBlock *c1;
  GmoImageBlock *b;
  u8 *addr = (u8 *)ptr;

  if (ptr == NULL) {
    return NULL;
  }
  c0 = p->cache0;
  if (c0 != NULL && addr < (u8 *)c0) {
    if ((u8 *)c0->alloc <= addr) {
      return (u32 *)c0;
    }
  }
  c1 = p->cache1;
  if (c1 != NULL && addr < (u8 *)c1) {
    if ((u8 *)c1->alloc <= addr) {
      p->cache1 = c0;
      p->cache0 = c1;
      return (u32 *)c1;
    }
  }
  if (addr < p->rangeLow || p->rangeHigh <= addr) {
    return NULL;
  }
  for (b = p->blocks; b != NULL; b = b->prev) {
    if (addr < (u8 *)b && (u8 *)b->alloc <= addr) {
      p->cache1 = c0;
      p->cache0 = b;
      return (u32 *)b;
    }
  }
  return NULL;
}
