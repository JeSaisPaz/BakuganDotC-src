// bdc 0x08a101d0 GmoImagePlanReserve
#include "bdc.h"

/* Measuring pass of an image-library allocation plan (`GmoImagePlan`): adds `size` (rounded up to
   the alignment of `align`'s class) to the class total of pool `pool`. Returns 1, or 0 (nothing
   reserved) when `plan` is NULL or `size` is 0. */

int GmoImagePlanReserve(void *plan, int pool, u32 align, int size)
{
  struct GmoImagePlan *p = (struct GmoImagePlan *)plan;
  u8 cls;
  u32 bytes;

  if (p == NULL || size == 0) {
    return 0;
  }
  cls = GmoImageAlignClass(align);
  bytes = GmoImageAlignClassBytes(cls);
  p->totals[pool][cls] += (size + bytes - 1) & -bytes;
  return 1;
}
