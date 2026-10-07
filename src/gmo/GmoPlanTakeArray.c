// bdc 0x08a133f0 GmoPlanTakeArray
#include "bdc.h"

/* Carves `count` records of `elemSize` bytes from a model-library plan (`GmoImagePlan` layout;
   inlined `GmoPlanTake`: the size is rounded up to `align`'s class (4/0x10/0x40/0x80) and taken
   from pool `pool`'s cursor of that class, adding a reference to the pool block), runs `ctor` on
   each record and adds `count - 1` further block references. Returns the array, or NULL when
   `plan` is NULL, `count * elemSize` is 0, the reservation is exhausted or the cursor was NULL. */

void *GmoPlanTakeArray(void *plan, int pool, u32 align, int elemSize, int count, void *ctor)
{
  GmoImagePlan *p = (GmoImagePlan *)plan;
  int size = count * elemSize;
  int cls;
  u32 bytes;
  u32 mask;
  u32 remain;
  u32 rounded;
  u8 *arr;
  u8 *elem;
  GmoImageBlock *blk;
  int i;

  if (plan == NULL || size == 0) {
    return NULL;
  }
  if (align < 5) {
    bytes = 4;
    cls = 3;
    mask = (u32)-4;
  } else if (align < 0x11) {
    bytes = 0x10;
    cls = 2;
    mask = (u32)-0x10;
  } else if (align < 0x41) {
    bytes = 0x40;
    cls = 1;
    mask = (u32)-0x40;
  } else {
    bytes = 0x80;
    cls = 0;
    mask = (u32)-0x80;
  }
  remain = (u32)p->totals[pool][cls];
  rounded = (bytes + (u32)size - 1) & mask;
  arr = p->cursors[pool][cls];
  if (remain < rounded) {
    return NULL;
  }
  p->cursors[pool][cls] = arr + rounded;
  p->totals[pool][cls] = (int)(remain - rounded);
  if (arr == NULL) {
    return NULL;
  }
  blk = p->blocks[pool];
  if (blk != NULL) {
    blk->refCount = (s16)(blk->refCount + 1);
  }
  if (ctor == NULL) {
    return arr;
  }
  if (count > 0) {
    elem = arr;
    for (i = 0; i < count; i++) {
      ((void (*)(void *))ctor)(elem);
      elem = elem + elemSize;
    }
  }
  if (count >= 2) {
    blk = p->blocks[pool];
    blk->refCount = (s16)(blk->refCount + count - 1);
  }
  return arr;
}
