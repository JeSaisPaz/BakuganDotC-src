// bdc 0x08a12f84 GmoPlanFree
#include "bdc.h"

/* Drops the plan's reference to each of its three pool blocks and zeroes the model-library
   allocation plan (0x6c bytes, `GmoImagePlan` layout). A block whose count reaches 0 is unlinked
   from its pool of the model heap (`g_gmoHeapPools`), the pool's lookup cache is fixed, and the
   block is freed with the pool's free function (or, when the pool's flag is set, parked on the
   pool's free list). Inlined counterpart of `GmoImageBlockRelease`. */

void GmoPlanFree(void *plan)
{
  GmoImagePlan *p = (GmoImagePlan *)plan;
  GmoImageBlock *b;
  GmoImageBlock *next;
  GmoImageBlock *prev;
  GmoImagePool *pool;
  u32 head;
  u32 tail;
  int i;

  if (plan == NULL) {
    return;
  }
  for (i = 0; i < 3; i++) {
    b = p->blocks[i];
    if (b == NULL) {
      continue;
    }
    b->refCount = (s16)(b->refCount - 1);
    if ((u16)b->refCount != 0) {
      continue;
    }
    pool = &g_gmoHeapPools[b->pool];
    next = b->next;
    if (next != NULL) {
      next->prev = b->prev;
    }
    prev = b->prev;
    if (prev != NULL) {
      prev->next = next;
    }
    if (b == pool->blocks) {
      pool->blocks = prev;
    }
    if (b == pool->cache1) {
      pool->cache1 = NULL;
    }
    if (b == pool->cache0) {
      next = pool->cache1;
      pool->cache1 = NULL;
      pool->cache0 = next;
    }
    head = g_gmoHeapBlockFreeHead;
    if (pool->flag != 0) {
      tail = g_gmoHeapBlockFreeTail;
      b->next = pool->freeList;
      b->pool = (u8)tail;
      b->offset = (u8)(tail >> 8);
      b->refCount = (s16)(tail >> 16);
      pool->freeList = b;
      memcpy(&b->prev, &head, 4);
    } else {
      pool->free(b->alloc);
    }
  }
  memset(plan, 0, 0x6c);
}
