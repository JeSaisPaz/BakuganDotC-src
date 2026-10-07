// bdc 0x08a12d64 GmoPlanReserve
#include "bdc.h"

/* Measuring pass of a model-library allocation plan (0x6c bytes, same layout as the image library's
   plan: 3 pool block pointers, 4 alignment-class totals per pool at `+0xc + pool * 0x10`, carve
   cursors at `+0x3c`): adds `size` rounded to the alignment class of `align` (4/0x10/0x40/0x80) to
   the class total of pool `pool`. Returns 1, or 0 when `plan` is NULL or `size` is 0. */

int GmoPlanReserve(void *plan, int pool, u32 align, int size)
{
  u32 *words = (u32 *)plan;
  u32 cls;
  u32 bytes;

  if (plan == NULL || size == 0) {
    return 0;
  }
  if (align < 5) {
    cls = 3;
    bytes = 4;
  } else if (align < 0x11) {
    cls = 2;
    bytes = 0x10;
  } else if (align < 0x41) {
    cls = 1;
    bytes = 0x40;
  } else {
    cls = 0;
    bytes = 0x80;
  }
  words[3 + pool * 4 + cls] += (bytes + size - 1) & -bytes;
  return 1;
}
