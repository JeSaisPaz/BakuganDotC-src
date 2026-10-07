// bdc 0x08a10380 GmoImagePlanTakeArray
#include "bdc.h"

/* Carves an array of `count` objects of `elemSize` bytes from pool `pool` of an image plan
   (`GmoImagePlanTake`). When the carve succeeds and `ctor` is non-NULL, runs `ctor` on each
   element and adds `count - 1` extra references to the pool block. Returns the array (NULL when
   the carve fails). */

void *GmoImagePlanTakeArray(void *plan, int pool, u32 align, int elemSize, int count, void *ctor)
{
  GmoImagePlan *p = (GmoImagePlan *)plan;
  void *arr;
  int i;
  u8 *elem;
  GmoImageBlock *blk;

  arr = GmoImagePlanTake(plan, pool, align, count * elemSize);
  if (arr != NULL && ctor != NULL) {
    elem = (u8 *)arr;
    for (i = 0; i < count; i++) {
      ((void (*)(void *))ctor)(elem);
      elem += elemSize;
    }
    if (count > 1) {
      blk = p->blocks[pool];
      blk->refCount = (short)(count + blk->refCount - 1);
    }
  }
  return arr;
}
