// bdc 0x0888a7b4 UiHpGaugeEmitBarSegment
#include "bdc.h"

/* Writes one shaded fill segment of the HUD HP gauge into the display list `dl` for
   `UiHpGaugeEmitBars`: packs `color` to ABGR4444 (`light`) and a half-bright copy (`dark`),
   writes two inline 5-vertex fans of `GfxGeColor4444Vertex16` (jumped over with BASE/JUMP),
   dark at `y0`/`y1` and light at the middle line y = -2, from `x0` to `x1` (widened by one when
   narrower than 2) at depth `z`; then sets the material colour to `g_colorWhite` and draws both
   fans (VTYPE/BASE/VADDR/PRIM fan of 5), sets TFUNC 0xc9000103 and writes ZTE off. Returns a
   pointer to that last ZTE word (not past it), so the caller's next command overwrites it.
   `self` is unused. Colours are clamped to [0, 1], scaled by 255 and truncated to bytes. */

u32 * UiHpGaugeEmitBarSegment(float x0, float y0, float x1, float y1, float z, UiHpGauge *self, u32 *dl, const ScePspFVector4 *color)

{
  u32 packed;
  u16 light;
  u16 dark;
  s16 ix0;
  s16 iy0;
  s16 ix1;
  s16 iy1;
  s16 iz;
  GfxGeColor4444Vertex16 (*top)[5];
  GfxGeColor4444Vertex16 (*bottom)[5];
  u32 *jump;
  u32 *cmd;
  ScePspFVector4 white;

  packed = (u32)VfI2uc(VfF2iz(VfSat0(color->x) * 255.0f, 23)) |
           (u32)VfI2uc(VfF2iz(VfSat0(color->y) * 255.0f, 23)) << 8 |
           (u32)VfI2uc(VfF2iz(VfSat0(color->z) * 255.0f, 23)) << 16 |
           (u32)VfI2uc(VfF2iz(VfSat0(color->w) * 255.0f, 23)) << 24;
  light = (u16)((packed >> 28) << 12 | ((packed >> 20) & 0xf) << 8 | ((packed >> 12) & 0xf) << 4 |
                ((packed >> 4) & 0xf));
  dark = (u16)((packed >> 28) << 12 | ((packed >> 22) & 7) << 8 | ((packed >> 14) & 7) << 4 |
               ((packed >> 6) & 7));
  ix0 = (s16)(int)x0;
  iy0 = (s16)(int)y0;
  ix1 = (s16)(int)x1;
  iy1 = (s16)(int)y1;
  if (ix1 - ix0 < 2) {
    ix1 = ix1 + 1;
  }

  /* first fan, jumped over: BASE + JUMP */
  top = (GfxGeColor4444Vertex16 (*)[5])(((uintptr_t)(dl + 2) + 0xf) & ~(uintptr_t)0xf);
  jump = (u32 *)&top[1];
  dl[0] = ((PspAddr(jump) >> 24) & 0xf) << 16 | 0x10000000;
  dl[1] = (PspAddr(jump) & 0xffffff) | 0x08000000;
  /* second fan, jumped over */
  bottom = (GfxGeColor4444Vertex16 (*)[5])(((uintptr_t)(jump + 2) + 0xf) & ~(uintptr_t)0xf);
  cmd = (u32 *)&bottom[1];
  jump[0] = ((PspAddr(cmd) >> 24) & 0xf) << 16 | 0x10000000;
  jump[1] = (PspAddr(cmd) & 0xffffff) | 0x08000000;

  iz = (s16)(int)z;
  (*top)[0].x = ix0; (*top)[0].y = iy0; (*top)[0].z = iz; (*top)[0].colour = dark;
  (*top)[1].x = ix1; (*top)[1].y = iy0; (*top)[1].z = iz; (*top)[1].colour = dark;
  (*top)[2].x = ix1; (*top)[2].y = -2;  (*top)[2].z = iz; (*top)[2].colour = light;
  (*top)[3].x = ix0; (*top)[3].y = -2;  (*top)[3].z = iz; (*top)[3].colour = light;
  (*top)[4].x = ix0; (*top)[4].y = iy0; (*top)[4].z = iz; (*top)[4].colour = dark;
  (*bottom)[0].x = ix0; (*bottom)[0].y = -2;  (*bottom)[0].z = iz; (*bottom)[0].colour = light;
  (*bottom)[1].x = ix1; (*bottom)[1].y = -2;  (*bottom)[1].z = iz; (*bottom)[1].colour = light;
  (*bottom)[2].x = ix1; (*bottom)[2].y = iy1; (*bottom)[2].z = iz; (*bottom)[2].colour = dark;
  (*bottom)[3].x = ix0; (*bottom)[3].y = iy1; (*bottom)[3].z = iz; (*bottom)[3].colour = dark;
  (*bottom)[4].x = ix0; (*bottom)[4].y = -2;  (*bottom)[4].z = iz; (*bottom)[4].colour = light;

  white = g_colorWhite;
  packed = (u32)VfI2uc(VfF2iz(VfSat0(white.x) * 255.0f, 23)) |
           (u32)VfI2uc(VfF2iz(VfSat0(white.y) * 255.0f, 23)) << 8 |
           (u32)VfI2uc(VfF2iz(VfSat0(white.z) * 255.0f, 23)) << 16 |
           (u32)VfI2uc(VfF2iz(VfSat0(white.w) * 255.0f, 23)) << 24;
  cmd[0] = (packed & 0xffffff) | 0x55000000; /* material colour */
  cmd[1] = (packed >> 24) | 0x58000000;      /* material alpha */
  cmd += 2;

  *cmd++ = 0x12000118; /* VTYPE: colour 4444, 16-bit position */
  if (top != NULL) {
    cmd[0] = ((PspAddr(top) >> 24) & 0xf) << 16 | 0x10000000; /* BASE */
    cmd[1] = (PspAddr(top) & 0xffffff) | 0x01000000;          /* VADDR */
    cmd += 2;
  }
  *cmd++ = 0x04050005; /* PRIM triangle fan, 5 vertices */
  *cmd++ = 0x12000118;
  if (bottom != NULL) {
    cmd[0] = ((PspAddr(bottom) >> 24) & 0xf) << 16 | 0x10000000;
    cmd[1] = (PspAddr(bottom) & 0xffffff) | 0x01000000;
    cmd += 2;
  }
  *cmd++ = 0x04050005;
  *cmd++ = 0xc9000103; /* TFUNC: modulate, RGBA */
  *cmd = 0x23000000;   /* ZTE off */
  return cmd;
}
