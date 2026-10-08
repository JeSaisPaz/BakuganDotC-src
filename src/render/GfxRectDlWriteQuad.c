// bdc 0x089ed78c GfxRectDlWriteQuad
#include "bdc.h"

/* Quad writer of the overlay rect (vtable `0x08af5734` slot `+0x14`): writes into `list` a
   BASE/JUMP pair skipping two inline 16-bit vertices (`x, y, z` from the float position `pos`,
   second corner offset by `width`/`height`; in 3D mode `mode == 1` the rect is centred by
   subtracting half the size from x/y), then VTYPE `0x12800100` (16-bit positions, through mode),
   BASE/VADDR pointing at the vertices and PRIM `0x04060002` (2-vertex sprite). Returns the
   advanced list pointer. */

u32 *GfxRectDlWriteQuad(void *rect, u32 *list)
{
  GfxRect *r = (GfxRect *)rect;
  s16 *verts = (s16 *)(list + 2); /* two {x, y, z} s16 vertices */
  u32 *end = list + 5;
  u32 addr;

  /* BASE + JUMP over the inline vertex data */
  list[0] = ((PspAddr(end) >> 0x18) & 0xf) << 0x10 | 0x10000000;
  list[1] = (PspAddr(end) & 0xffffff) | 0x8000000;
  if (r->mode == 1) {
    verts[0] = (s16)(s32)((float)((s32)r->pos[0] & 0xffff) - (float)r->width * 0.5f);
    verts[1] = (s16)(s32)((float)((s32)r->pos[1] & 0xffff) - (float)r->height * 0.5f);
  } else {
    verts[0] = (s16)(s32)r->pos[0];
    verts[1] = (s16)(s32)r->pos[1];
  }
  verts[2] = (s16)(s32)r->pos[2];
  verts[3] = verts[0];
  verts[4] = verts[1];
  verts[5] = verts[2];
  verts[3] = (s16)((u16)verts[3] + r->width);
  verts[4] = (s16)((u16)verts[4] + r->height);

  list = end;
  *list++ = 0x12800100; /* VTYPE: 16-bit position, through mode */
  if (verts != NULL) {
    addr = PspAddr(verts);
    list[0] = ((addr >> 0x18) & 0xf) << 0x10 | 0x10000000;
    list[1] = (addr & 0xffffff) | 0x1000000; /* VADDR */
    list += 2;
  }
  *list = 0x4060002; /* PRIM: sprites, 2 vertices */
  return list + 1;
}
