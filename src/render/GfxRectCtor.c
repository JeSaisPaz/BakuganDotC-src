// bdc 0x089ed698 GfxRectCtor
#include "bdc.h"

/* Constructor of the 0x50-byte overlay rect (vtable `0x08af5734`): base fields
   (`GfxRectBaseCtor`), draw mode `+0x30` (0 = 2D, 1 = 3D), size `+0x34`/`+0x38` and position
   `+0x40..+0x4c` cleared. */

void *GfxRectCtor(void *rect, s32 mode)
{
  GfxRect *r = (GfxRect *)rect;
  GfxRect tmp;

  GfxRectBaseCtor(r);
  r->vtbl = g_gfxRectVtbl;
  r->mode = mode;
  r->height = 0;
  r->width = 0;
  r->pos[0] = 0.0f;
  r->pos[1] = 0.0f;
  r->pos[2] = 0.0f;
  r->pos[3] = 0.0f;
  GfxRectBaseCtor(&tmp);
  GfxRectBaseDtor(&tmp, 2);
  return rect;
}
