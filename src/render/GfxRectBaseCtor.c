// bdc 0x089ed2f0 GfxRectBaseCtor
#include "bdc.h"

/* Constructor of the base overlay rect (vtable `g_gfxRectBaseVtbl` at `+0x28`): mode 0, hidden,
   colour (1, 1, 1, 1) at `+0x10`, flags `+0x20` cleared. */

GfxRect *GfxRectBaseCtor(GfxRect *rect)
{
  rect->vtbl = g_gfxRectBaseVtbl;
  rect->stateMode = 0;
  rect->visibleByte = 0;
  rect->color[0] = 1.0f;
  rect->color[1] = 1.0f;
  rect->color[2] = 1.0f;
  rect->color[3] = 1.0f;
  rect->flags = 0;
  return rect;
}
