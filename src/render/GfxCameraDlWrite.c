// bdc 0x089e2fe4 GfxCameraDlWrite
#include "bdc.h"

/* Writes a camera's GE state into the display list: `flags & 1` the view matrix (`VIEW_START`
   `0x3c` + 12 `0x3d` data words: the x/y/z of each of the four rows of `view`), `& 2` the
   projection (`GfxDlWriteProjMatrix` of `proj`), `& 4` the depth range (`0x44` ZSCALE
   `(min+max)/2 - max`, `0x47` ZPOS `g_gfxDepthCenter` = `fogBase + (min+max)/2`, `0xd6`/`0xd7`
   the smaller/larger of `depthMin`/`depthMax` truncated to int). Any camera other than
   `g_gfxScreenCamera` becomes `g_gfxActiveCamera`. Returns the advanced list pointer. */

u32 *GfxCameraDlWrite(GfxCamera *cam, u32 *list, u32 flags)
{
  const u32 *w;
  union {
    float f;
    u32 u;
  } bits;
  float center;
  float scale;
  s32 zmin;
  s32 zmax;
  u32 lo;
  u32 hi;

  if ((flags & 1) != 0) {
    w = (const u32 *)&cam->view;
    list[0] = 0x3c000000;
    /* 0x3d000000 is the .rodata word 0x08aa31f4; the asm merges each float's top 24 bits into it
       with byte-offset-1 `lwr`s. */
    list[1] = 0x3d000000 | (w[0] >> 8);
    list[2] = 0x3d000000 | (w[1] >> 8);
    list[3] = 0x3d000000 | (w[2] >> 8);
    list[4] = 0x3d000000 | (w[4] >> 8);
    list[5] = 0x3d000000 | (w[5] >> 8);
    list[6] = 0x3d000000 | (w[6] >> 8);
    list[7] = 0x3d000000 | (w[8] >> 8);
    list[8] = 0x3d000000 | (w[9] >> 8);
    list[9] = 0x3d000000 | (w[10] >> 8);
    list[10] = 0x3d000000 | (w[12] >> 8);
    list[11] = 0x3d000000 | (w[13] >> 8);
    list[12] = 0x3d000000 | (w[14] >> 8);
    list = list + 13;
  }
  if ((flags & 2) != 0) {
    list = GfxDlWriteProjMatrix(list, &cam->proj);
  }
  if ((flags & 4) != 0) {
    zmax = (s32)cam->depthMax;
    zmin = (s32)cam->depthMin;
    if (cam->depthMin <= cam->depthMax) {
      lo = (u32)zmin | 0xd6000000;
      hi = (u32)zmax | 0xd7000000;
    } else {
      lo = (u32)zmax | 0xd6000000;
      hi = (u32)zmin | 0xd7000000;
    }
    center = (cam->depthMin + cam->depthMax) * 0.5f;
    scale = center - cam->depthMax;
    g_gfxDepthCenter = cam->fogBase + center;
    bits.f = scale;
    list[0] = (bits.u >> 8) | 0x44000000;
    bits.f = g_gfxDepthCenter;
    list[2] = lo;
    list[1] = (bits.u >> 8) | 0x47000000;
    list[3] = hi;
    list = list + 4;
  }
  if (cam != g_gfxScreenCamera) {
    g_gfxActiveCamera = cam;
  }
  return list;
}
