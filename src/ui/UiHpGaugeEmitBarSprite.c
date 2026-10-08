// bdc 0x0888a3b0 UiHpGaugeEmitBarSprite
#include "bdc.h"

/* Writes one textured sprite of the HUD HP gauge (0xa0-byte node,
   `UiHpGaugeInit`/`UiHpGaugeCtorForObject`, mode `+0x8c`) into the display list: GE state words
   (`0xdf000032`, `0xe0000000`, `0xe1000000`), the colour from the vec4 `color` (saturated, scaled by
   255, packed to RGBA: AMBIENT colour + alpha), texture on, unit TEXSCALE / zero
   TEXOFFSET, the texture (`GfxTextureWriteCall`, slot 0), then a through-mode 2-vertex sprite (inline
   `GfxGeTexVertex16` behind a JUMP) spanning `x0..x1` / `y0..y1` (truncated to 16-bit integers,
   `x1` widened by one when the span is under 2) around the truncated gauge origin `screenPos`. The
   texel rectangle is (0, 0)..(u, 16) with `u = span / UiHpGaugeGetBarWidth * 128 * maxHp /
   UiHpGaugeGetScaleMax`. Finishes with texture off and the TEXSCALE/TEXOFFSET words again and
   returns the new write pointer. */

u32 *UiHpGaugeEmitBarSprite(float x0, float y0, float x1, float y1, UiHpGauge *self, u32 *dl, void *color)
{
  s16 ix0;
  s16 iy0;
  s16 ix1;
  s16 iy1;
  u32 packed;
  u32 *list;
  u32 *end;
  GfxGeTexVertex16 *verts;
  GfxGeTexVertex16 *second;
  float originX;
  float originY;
  float u;
  int i;
  float corners[4];
  float uv[4];
  s32 xy[4];
  s32 st[4];

  ix0 = (s16)(int)x0;
  iy0 = (s16)(int)y0;
  ix1 = (s16)(int)x1;
  iy1 = (s16)(int)y1;
  if (ix1 - ix0 < 2) {
    ix1 = (s16)(ix1 + 1);
  }
  dl[0] = 0xdf000032;
  dl[1] = 0xe0000000;
  dl[2] = 0xe1000000;
  {
    const float *rgba = (const float *)color;

    packed = (u32)VfI2uc(VfF2iz(VfSat0(rgba[0]) * 255.0f, 23)) |
             (u32)VfI2uc(VfF2iz(VfSat0(rgba[1]) * 255.0f, 23)) << 8 |
             (u32)VfI2uc(VfF2iz(VfSat0(rgba[2]) * 255.0f, 23)) << 16 |
             (u32)VfI2uc(VfF2iz(VfSat0(rgba[3]) * 255.0f, 23)) << 24;
  }
  dl[3] = (packed & 0xffffff) | 0x55000000; /* AMBIENT colour */
  dl[4] = (packed >> 0x18) | 0x58000000;    /* AMBIENT alpha */
  dl[5] = 0x1e000001;                       /* texture mapping on */
  dl[6] = 0x23000000;
  dl[7] = 0x48000000 | 0x3f8000; /* TEXSCALE u = 1.0f (float24) */
  dl[8] = 0x49000000 | 0x3f8000; /* TEXSCALE v = 1.0f */
  dl[9] = 0x4a000000;            /* TEXOFFSET u = 0.0f */
  dl[10] = 0x4b000000;           /* TEXOFFSET v = 0.0f */
  list = GfxTextureWriteCall(self->texture, dl + 11, 0);
  originX = (float)(int)self->screenPos[0];
  originY = (float)(int)self->screenPos[1];
  corners[0] = originX + (float)ix0;
  corners[1] = originY + (float)iy0;
  corners[2] = originX + (float)ix1;
  corners[3] = originY + (float)iy1;
  uv[0] = 0.0f;
  uv[1] = 0.0f;
  uv[2] = 0.0f;
  uv[3] = 16.0f;
  u = ((corners[2] - corners[0]) / UiHpGaugeGetBarWidth(self)) * 128.0f;
  uv[2] = u;
  uv[2] = u * (self->maxHp / UiHpGaugeGetScaleMax(self));
  for (i = 0; i < 4; i++) {
    xy[i] = VfF2iz(corners[i], 0);
    st[i] = VfF2iz(uv[i], 0);
  }
  verts = (GfxGeTexVertex16 *)(list + 2);
  second = verts + 1;
  end = (u32 *)(second + 1);
  /* BASE + JUMP over the inline vertex data */
  list[0] = ((PspAddr(end) >> 0x18) & 0xf) << 0x10 | 0x10000000;
  list[1] = (PspAddr(end) & 0xffffff) | 0x8000000;
  verts[0].u = st[0];
  verts[0].v = st[1];
  second->u = st[2];
  second->v = st[3];
  verts[0].x = xy[0];
  verts[0].y = xy[1];
  second->x = xy[2];
  second->y = xy[3];
  second->z = 0;
  verts[0].z = 0;
  list = end;
  *list++ = 0x12800102; /* VTYPE: 16-bit texel, 16-bit position, through mode */
  if (verts != NULL) {
    list[0] = ((PspAddr(verts) >> 0x18) & 0xf) << 0x10 | 0x10000000;
    list[1] = (PspAddr(verts) & 0xffffff) | 0x1000000; /* VADDR */
    list += 2;
  }
  list[0] = 0x4060002;           /* PRIM sprites, 2 vertices */
  list[1] = 0x1e000000;          /* texture mapping off */
  list[2] = 0x48000000 | 0x3f8000; /* TEXSCALE u = 1.0f */
  list[3] = 0x49000000 | 0x3f8000; /* TEXSCALE v = 1.0f */
  list[4] = 0x4a000000;          /* TEXOFFSET u = 0.0f */
  list[5] = 0x4b000000;          /* TEXOFFSET v = 0.0f */
  return list + 6;
}
