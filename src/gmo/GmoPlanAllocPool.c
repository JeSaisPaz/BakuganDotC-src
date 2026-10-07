// bdc 0x08a12a50 GmoPlanAllocPool
#include "bdc.h"

/* Allocates the block for pool `pool` of a model-library allocation plan (`GmoImagePlan` layout)
   from the summed alignment-class totals (`sizes`, or the plan's own `totals[pool]`), aligning to
   the largest class used (0x80/0x40/0x10/4). The model-heap block allocation is inlined: calls the
   pool allocator of `g_gmoHeapPools` for size + alignment slack + a 0x10-byte `GmoImageBlock`
   trailer, links the trailer at the head of the pool's block list and widens the pool's address
   range. Stores the block in `blocks[pool]` and sets the four carve cursors of that pool to
   consecutive spans of the block's data. Returns 1 on success or when the total is 0, 0 when the
   allocation fails. */

int GmoPlanAllocPool(void *plan, int pool, int *sizes)
{
  GmoImagePlan *p = (GmoImagePlan *)plan;
  GmoImagePool *hp;
  GmoImageBlock *block;
  GmoImageBlock *head;
  u32 align;
  u32 poolAlign;
  u32 effAlign;
  u32 padding;
  u8 *raw;
  u8 *data;
  u8 *base;
  int total;

  if (sizes == NULL) {
    sizes = p->totals[pool];
  }
  if (sizes[0] != 0) {
    align = 0x80;
  } else if (sizes[1] != 0) {
    align = 0x40;
  } else if (sizes[2] != 0) {
    align = 0x10;
  } else {
    align = 4;
  }
  total = sizes[0] + sizes[1] + sizes[2] + sizes[3];
  if (total == 0) {
    return 1;
  }

  hp = &g_gmoHeapPools[pool];
  poolAlign = hp->align;
  effAlign = poolAlign;
  padding = 3;
  if (poolAlign <= align) {
    effAlign = align;
  }
  if (effAlign > 3) {
    padding = -total & 3;
  }
  raw = hp->alloc(padding + (total + (effAlign - poolAlign)) + 0x10);
  if (raw == NULL) {
    return 0;
  }
  data = (u8 *)(((uintptr_t)raw + align - 1) & -(uintptr_t)align);
  block = (GmoImageBlock *)(((uintptr_t)(data + total) + 3) & ~(uintptr_t)3);

  head = hp->blocks;
  hp->blocks = block;
  if (head != NULL) {
    head->next = block;
  }
  if (hp->rangeLow == NULL || data < hp->rangeLow) {
    hp->rangeLow = data;
  }
  if (hp->rangeHigh == NULL || hp->rangeHigh < (u8 *)block) {
    hp->rangeHigh = (u8 *)block;
  }
  block->offset = (u8)(data - raw);
  block->pool = (u8)pool;
  block->alloc = raw;
  p->blocks[pool] = block;

  base = (u8 *)block->alloc + block->offset;
  p->cursors[pool][3] = base + sizes[0] + sizes[1] + sizes[2];
  block->prev = head;
  block->refCount = 1;
  block->next = NULL;
  p->cursors[pool][0] = base;
  p->cursors[pool][1] = base + sizes[0];
  p->cursors[pool][2] = base + sizes[0] + sizes[1];
  return 1;
}
