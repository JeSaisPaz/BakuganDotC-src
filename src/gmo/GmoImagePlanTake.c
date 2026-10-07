// bdc 0x08a102a4 GmoImagePlanTake
#include "bdc.h"

/* Carving pass of an image-library allocation plan (`GmoImagePlan`): takes `size` bytes of
   alignment class `align` from pool `pool`'s cursor (`cursors[pool][cls]`), decrementing the
   remaining total (`totals[pool][cls]`) and referencing the block. Returns the pointer, or 0 for a
   NULL plan, a zero size, or when the reservation is exhausted. */

void *GmoImagePlanTake(void *plan, int pool, u32 align, int size)
{
  GmoImagePlan *p = (GmoImagePlan *)plan;
  u32 cls;
  u32 bytes;
  u32 remain;
  u32 rounded;
  u8 *ptr;

  if (p == NULL || size == 0) {
    return NULL;
  }
  cls = GmoImageAlignClass(align);
  bytes = GmoImageAlignClassBytes(cls);
  remain = (u32)p->totals[pool][cls];
  rounded = (size + bytes - 1) & -bytes;
  ptr = p->cursors[pool][cls];
  if (remain < rounded) {
    return NULL;
  }
  p->cursors[pool][cls] = ptr + rounded;
  p->totals[pool][cls] = (int)(remain - rounded);
  GmoImagePlanAddRef(plan, pool, ptr);
  return ptr;
}
