// bdc 0x089f300c GfxPacketDrawGradientBands
#include "bdc.h"

/* Draws full-width (0..480) horizontal colour bands into `packet` as a triangle strip: `count + 1`
   rows at the Y values `ys[]` (s16) with the RGBA colours `colours[]`, blend preset `blend`. The
   vertices sit inline in the chunk behind a JUMP; the state comes from `GfxDlSetBlendState` with
   `g_colorWhite` and no texture. Used for sky/background gradients. */

void GfxPacketDrawGradientBands(void *packet, const s16 *ys, const u32 *colours, s32 count, s32 blend)
{
  u32 *list;
  u32 *end;
  GfxGeColorVertex16 *verts;
  GfxGeColorVertex16 *vtx;
  s32 vcount;
  s32 i;

  list = GfxPacketBeginChunk(packet);
  verts = (GfxGeColorVertex16 *)(list + 2);
  end = (u32 *)verts + (((count + 1) * 0x18 + 3) >> 2);
  /* BASE + JUMP over the inline vertex data */
  list[0] = (((uintptr_t)end >> 0x18) & 0xf) << 0x10 | 0x10000000;
  list[1] = ((uintptr_t)end & 0xffffff) | 0x8000000;
  vcount = count * 2 + 2;
  vtx = verts;
  for (i = 0; i < vcount; i++) {
    vtx->x = (i & 1) ? 0x1e0 : 0;
    vtx->y = ys[i / 2];
    vtx->z = 0;
    vtx->colour = colours[i / 2];
    vtx++;
  }
  list = GfxDlSetBlendState(end, &g_colorWhite, 0, blend);
  *list++ = 0x1280011c; /* VTYPE: colour 8888, 16-bit position, through mode */
  if (verts != NULL) {
    list[0] = (((uintptr_t)verts >> 0x18) & 0xf) << 0x10 | 0x10000000;
    list[1] = ((uintptr_t)verts & 0xffffff) | 0x1000000; /* VADDR */
    list += 2;
  }
  *list = vcount | 0x4040000; /* PRIM triangle strip */
  GfxPacketEndChunk(packet, list + 1);
}
