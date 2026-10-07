// bdc 0x08a10038 GmoImagePlanInit
#include "bdc.h"

/* Zeroes an image-library allocation plan (0x6c bytes: 3 pool block pointers, per pool 4
   alignment-class byte totals at `+0xc + pool * 0x10`, and the matching carve cursors at `+0x3c`).
    */

void GmoImagePlanInit(void *plan)

{
  if (plan != (void *)0x0) {
    memset(plan,0,0x6c);
    return;
  }
  return;
}

