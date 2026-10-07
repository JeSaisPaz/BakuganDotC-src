// bdc 0x08a10454 GmoImageBlockRelease
#include "bdc.h"

/* Drops a reference of an image-heap block; at 0 unlinks it from its pool of the image library's
   3-pool block heap (pool records of 0x24 bytes at `0x08af1258`: allocator, free function,
   alignment, flags, block list, lookup cache, address range) (fixing the lookup cache) and frees it
   with the pool's free function (or, when the pool's `flag` is set, parks it on the pool's
   `freeList`). Returns the new count, zero-extended (0xffff when an unreferenced block underflows);
   0 for NULL. */

u16 GmoImageBlockRelease(void *block)
{
  GmoImageBlock *b = (GmoImageBlock *)block;
  GmoImagePool *p;
  GmoImageBlock *next;
  GmoImageBlock *prev;
  u32 tail;
  u32 head;
  u16 count = 0;

  if (b != NULL) {
    count = (u16)(b->refCount - 1);
    b->refCount = (s16)count;
    if (count == 0) {
      p = &g_gmoImagePools[b->pool];
      next = b->next;
      if (next != NULL) {
        next->prev = b->prev;
      }
      prev = b->prev;
      if (prev != NULL) {
        prev->next = next;
      }
      if (b == p->blocks) {
        p->blocks = prev;
      }
      if (b == p->cache1) {
        p->cache1 = NULL;
      }
      if (b == p->cache0) {
        next = p->cache1;
        p->cache1 = NULL;
        p->cache0 = next;
      }
      if (p->flag != 0) {
        head = g_gmoImageBlockFreeHead;
        tail = g_gmoImageBlockFreeTail;
        memcpy(&b->prev, &head, 4);
        b->pool = (u8)tail;
        b->offset = (u8)(tail >> 8);
        b->refCount = (s16)(tail >> 16);
        b->next = p->freeList;
        p->freeList = b;
        return 0;
      }
      p->free(b->alloc);
    }
  }
  return count;
}
