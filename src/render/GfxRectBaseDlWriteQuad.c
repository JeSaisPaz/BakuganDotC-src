// bdc 0x08a32488 GfxRectBaseDlWriteQuad
#include "bdc.h"

/* Quad-writer virtual of the base overlay rect (vtable `0x08af571c` slot `+0x14`): writes nothing
   and returns `list` unchanged. */

u32 *GfxRectBaseDlWriteQuad(void *rect, u32 *list)

{
  return list;
}

