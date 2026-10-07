// bdc 0x08a136c0 GmoPlanCommit
#include "bdc.h"

/* Allocates the blocks for the measured totals of a model-library allocation plan (0x6c bytes,
   `GmoImagePlan` layout), releasing any previous blocks first (the plan's `blocks` are dropped
   and cleared). When pool 1 of `g_gmoHeapPools` has its `custom` flag set, each of the three
   pools gets its own block (`GmoPlanAllocPool`); otherwise pools 0 and 1 share one pool-0 block
   sized for both pools' alignment-class totals, whose reference count is bumped for the second
   slot, and pool 1's carve cursors start right after pool 0's spans. Returns 1 on success; on
   failure drops the plan's references again, zeroes the plan and returns 0 (also 0 for a NULL
   plan). */

/* Drops one reference to a model-heap block; at 0 it is unlinked from its pool, the pool's lookup
   cache is fixed and the block is freed (or parked on the pool's free list). Inlined three times
   in the original, same body as `GmoPlanFree`. */
static inline void GmoPlanCommitDropBlock(GmoImageBlock *b)
{
  GmoImageBlock *next;
  GmoImageBlock *prev;
  GmoImagePool *pool;
  u32 head;
  u32 tail;

  b->refCount = (s16)(b->refCount - 1);
  if ((u16)b->refCount != 0) {
    return;
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

int GmoPlanCommit(int *plan)
{
  GmoImagePlan *p = (GmoImagePlan *)plan;
  GmoImageBlock *shared;
  int sizes[4];
  int i;

  if (plan == NULL) {
    return 0;
  }
  for (i = 0; i < 3; i++) {
    if (p->blocks[i] != NULL) {
      GmoPlanCommitDropBlock(p->blocks[i]);
    }
    p->blocks[i] = NULL;
  }

  if (g_gmoHeapPools[1].custom == 0) {
    for (i = 0; i < 4; i++) {
      sizes[i] = p->totals[0][i] + p->totals[1][i];
    }
    if (GmoPlanAllocPool(plan, 0, sizes) != 0 && GmoPlanAllocPool(plan, 2, NULL) != 0) {
      shared = p->blocks[0];
      p->blocks[1] = shared;
      if (shared != NULL) {
        shared->refCount = (s16)(shared->refCount + 1);
      }
      for (i = 0; i < 4; i++) {
        p->cursors[1][i] = p->cursors[0][i] + p->totals[0][i];
      }
      return 1;
    }
  } else {
    if (GmoPlanAllocPool(plan, 0, NULL) != 0 && GmoPlanAllocPool(plan, 1, NULL) != 0 &&
        GmoPlanAllocPool(plan, 2, NULL) != 0) {
      return 1;
    }
  }

  for (i = 0; i < 3; i++) {
    if (p->blocks[i] != NULL) {
      GmoPlanCommitDropBlock(p->blocks[i]);
    }
  }
  memset(plan, 0, sizeof(GmoImagePlan));
  return 0;
}
