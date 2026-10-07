// bdc 0x08a130f0 GmoHeapAddRef
#include "bdc.h"

/* Adds a reference to the block of the model library's 3-pool block heap (`g_gmoHeapPools`; same
   design as the image library's `GmoImageHeapFindBlock` heap) that holds `ptr`, searching pool
   `pool` first and then the next pools (mod 3). In each pool it checks cache0 (hit: no cache
   update), cache1 (hit: swapped into cache0), then the pool's address range and block list (hit:
   becomes cache0, old cache0 moves to cache1). A pointer found in no pool is left alone. Returns
   `ptr` (NULL is passed through). */

void *GmoHeapAddRef(int pool, void *ptr)
{
  u8 *addr = (u8 *)ptr;
  GmoImagePool *p;
  GmoImageBlock *c0;
  GmoImageBlock *c1;
  GmoImageBlock *b;
  int i;

  if (ptr == NULL) {
    return ptr;
  }
  for (i = 0; i != 3; i++) {
    p = &g_gmoHeapPools[pool];
    c0 = p->cache0;
    if (c0 != NULL && addr < (u8 *)c0 && (u8 *)c0->alloc <= addr) {
      c0->refCount++;
      return ptr;
    }
    c1 = p->cache1;
    if (c1 != NULL && addr < (u8 *)c1 && (u8 *)c1->alloc <= addr) {
      b = c1;
      goto found;
    }
    if (!(addr < p->rangeLow) && addr < p->rangeHigh) {
      for (b = p->blocks; b != NULL; b = b->prev) {
        if (addr < (u8 *)b && (u8 *)b->alloc <= addr) {
          goto found;
        }
      }
    }
    pool = (pool + 1) % 3;
  }
  return ptr;

found:
  p->cache1 = c0;
  p->cache0 = b;
  b->refCount++;
  return ptr;
}
