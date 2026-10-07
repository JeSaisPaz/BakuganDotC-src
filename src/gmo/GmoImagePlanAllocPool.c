// bdc 0x08a10054 GmoImagePlanAllocPool
#include "bdc.h"

/* Allocates the block for pool `pool` of an image-library allocation plan (`GmoImagePlan`) from
   the summed alignment-class totals (`sizes`, or the plan's own `totals[pool]`), aligning to the
   largest class used (0x80/0x40/0x10/4), stores the block in `blocks[pool]` and sets the four carve
   cursors of that pool to consecutive spans of the block's data. Returns 1 on success or when the
   total is 0, 0 when the allocation fails. */

int GmoImagePlanAllocPool(void *plan, int pool, int *sizes)
{
  GmoImagePlan *p = (GmoImagePlan *)plan;
  GmoImageBlock *block;
  u8 *base;
  u32 align;
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
  block = (GmoImageBlock *)GmoImageHeapAllocBlock(pool, align, total);
  if (block == NULL) {
    return 0;
  }
  p->blocks[pool] = block;
  base = (u8 *)block->alloc + block->offset;
  p->cursors[pool][3] = base + sizes[0] + sizes[1] + sizes[2];
  p->cursors[pool][0] = base;
  p->cursors[pool][1] = base + sizes[0];
  p->cursors[pool][2] = base + sizes[0] + sizes[1];
  return 1;
}
