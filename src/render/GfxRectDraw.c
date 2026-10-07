// bdc 0x089ed3d4 GfxRectDraw
#include "bdc.h"

/* Draw method of the overlay rect object (`GfxRectCtor`): when visible (`flags & 1`,
   `GfxRectSetVisible`) and its alpha `color[3]` is non-zero, writes the render state chosen by
   `stateMode` (0: `GfxDlCall2DState`; 1: identity world matrix `0x3a`/`0x3b` from
   `g_gfxIdentityMatrix`, the active camera via `GfxCameraDlWrite` when there is one, lighting
   through `GfxDlWriteLightState` and a CALL of `g_gfxRect3DStateList`; anything else: none),
   then alpha blending off, the stencil setup from `flags` bits 2/1 (write `stencilRef` / test
   EQUAL `stencilRef`, otherwise stencil off), blend `0xdf000032` with zero fix colours, the
   clamped colour packed into the ambient colour/alpha commands, and finally the quad through the
   virtual quad writer (vtable slot 2, `GfxRectDlWriteQuad`). Nothing is written otherwise. */

void GfxRectDraw(void *rect, u32 *list)
{
  GfxRect *r = (GfxRect *)rect;
  const u32 *m;
  s32 mode;
  int row, col;
  uintptr_t addr;
  u32 packed;

  if ((r->flags & 1) == 0 || r->color[3] == 0.0f) {
    return;
  }
  mode = r->stateMode;
  if (mode > 0) {
    if (mode < 2) {
      /* world matrix = identity (the 0x3b command byte comes from the read-only word 0x08aa37e8) */
      m = (const u32 *)&g_gfxIdentityMatrix;
      list[0] = 0x3a000000;
      for (row = 0; row < 4; row++) {
        for (col = 0; col < 3; col++) {
          list[1 + row * 3 + col] = m[row * 4 + col] >> 8 | 0x3b000000;
        }
      }
      list += 13;
      if (g_gfxActiveCamera != NULL) {
        list = GfxCameraDlWrite(g_gfxActiveCamera, list, 0xffffffff);
      }
      list = GfxDlWriteLightState(list, g_gfxActiveCamera, 0);
      addr = (uintptr_t)g_gfxRect3DStateList;
      list[0] = ((addr >> 0x18) & 0xf) << 0x10 | 0x10000000; /* BASE */
      list[1] = (addr & 0xffffff) | 0xa000000;                /* CALL */
      list += 2;
    }
  } else if (mode >= 0) {
    list = GfxDlCall2DState(list);
  }

  *list++ = 0x1e000000; /* alpha blend off */
  if (r->flags & 4) {
    list[0] = 0x24000001;                        /* stencil test on */
    list[1] = 0xdd020000;                        /* SOP: replace on pass */
    list[2] = r->stencilRef << 8 | 0xdcff0001;   /* STST: ALWAYS, ref, mask 0xff */
    list += 3;
  } else if (r->flags & 2) {
    list[0] = 0x24000001;
    list[1] = 0xdd000000;                        /* SOP: keep */
    list[2] = r->stencilRef << 8 | 0xdcff0002;   /* STST: EQUAL, ref, mask 0xff */
    list += 3;
  } else {
    *list++ = 0x24000000; /* stencil test off */
  }
  list[0] = 0xdf000032;
  list[1] = 0xe0000000;
  list[2] = 0xe1000000;
  list += 3;
  /* colour clamped to [0, 1], scaled by 255 (bank constant S701) and truncated to bytes, red low */
  packed = (u32)VfI2uc(VfF2iz(VfSat0(r->color[0]) * 255.0f, 23)) |
           (u32)VfI2uc(VfF2iz(VfSat0(r->color[1]) * 255.0f, 23)) << 8 |
           (u32)VfI2uc(VfF2iz(VfSat0(r->color[2]) * 255.0f, 23)) << 16 |
           (u32)VfI2uc(VfF2iz(VfSat0(r->color[3]) * 255.0f, 23)) << 24;
  list[0] = (packed & 0xffffff) | 0x55000000;
  list[1] = (packed >> 0x18) | 0x58000000;
  list += 2;
  ((u32 * (*)(void *, u32 *)) r->vtbl[2].fn)((char *)r + r->vtbl[2].delta, list);
}
