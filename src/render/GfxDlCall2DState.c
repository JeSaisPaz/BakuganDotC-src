// bdc 0x089f13e8 GfxDlCall2DState
#include "bdc.h"

/* Writes a GE `CALL` to the static 2D render-state list at `0x08aa39e4` and returns the advanced
   list pointer. */

u32 *GfxDlCall2DState(u32 *list)

{
  *list = 0x10080000;
  list[1] = 0xaaa39e4;
  return list + 2;
}

