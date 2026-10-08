// bdc 0x0888ab58 UiHpGaugeEmitBars
#include "bdc.h"

/* Emits the bars of the HUD HP gauge (0xa0-byte node, `UiHpGaugeInit`/`UiHpGaugeCtorForObject`)
   into the display list `dl` and returns the new write pointer. `scale` is the bar length; -1.0
   (what `UiHpGaugeDraw` passes) means "use `UiHpGaugeGetBarWidth`". The bar spans
   x = ±scale/2 and y = -h .. h+1 (h = 7 for the local player, else 3). In order:
   a translucent black background sprite (alpha × 0.5, z -100); for segment 0, when the trail
   value `hpTrail` is > 0 in that segment, the fill up to `hp` in the style colour of
   `UiHpGaugeGetBarStyle` (style 0: `UiHpGaugeEmitBarSprite`, else
   `UiHpGaugeEmitBarSegment` at z -30) and, while `hitTimer` > 0, a white sprite from the fill
   end to the trail end (alpha hitTimer × alpha, z -20); a red low-HP flash sprite
   (alpha lowHpFlash × alpha, z -10); and finally a 9-vertex ABGR4444 line strip outlining the bar
   (grey/white, alpha × 15). */

/* lv.q/vsat0.q/vscl.q S701 (255)/vf2iz.q 23/vi2uc.q: RGBA floats to ABGR8888 bytes */
static inline u32 UiHpGaugePackColor(const ScePspFVector4 *c)
{
  return (u32)VfI2uc(VfF2iz(VfSat0(c->x) * 255.0f, 23)) |
         (u32)VfI2uc(VfF2iz(VfSat0(c->y) * 255.0f, 23)) << 8 |
         (u32)VfI2uc(VfF2iz(VfSat0(c->z) * 255.0f, 23)) << 16 |
         (u32)VfI2uc(VfF2iz(VfSat0(c->w) * 255.0f, 23)) << 24;
}

u32 *UiHpGaugeEmitBars(float scale, UiHpGauge *self, u32 *dl)

{
  ScePspFVector4 colors[7] __attribute__((aligned(16)));
  ScePspFVector4 spriteColor __attribute__((aligned(16)));
  ScePspFVector4 segmentColor __attribute__((aligned(16)));
  s32 style;
  s32 segment;
  float halfHeight;
  float x0;
  float y0;
  float x1;
  float y1;
  float pxPerHp;
  float fillX;
  float trail;
  int ix0;
  int iy0;
  int ix1;
  int iy1;
  s16 midX;
  s16 midY;
  u16 shade;
  u16 dark;
  u16 light;
  u32 packed;
  int i;
  GfxGeVertex16 (*rect)[2];
  GfxGeColor4444Vertex16 (*outline)[9];
  u32 *cmd;

  /* bar style colours 0..3 */
  colors[0].x = 1.0f;
  colors[0].y = 1.0f;
  colors[0].z = 1.0f;
  colors[0].w = self->alpha;
  colors[1].x = 0.95f;
  colors[1].y = 0.2f;
  colors[1].z = 0.95f;
  colors[1].w = self->alpha;
  colors[2].x = 1.0f;
  colors[2].y = 0.90625f;
  colors[2].z = 0.265625f;
  colors[2].w = self->alpha;
  colors[3].x = 0.0f;
  colors[3].y = 0.84375f;
  colors[3].z = 0.90625f;
  colors[3].w = self->alpha;
  /* hit flash (white), background (black), low-HP flash (red) */
  colors[4].x = 1.0f;
  colors[4].y = 1.0f;
  colors[4].z = 1.0f;
  colors[4].w = 1.0f;
  colors[5].x = 0.0f;
  colors[5].y = 0.0f;
  colors[5].z = 0.0f;
  colors[5].w = self->alpha * 0.5f;
  colors[6].x = 1.0f;
  colors[6].y = 0.0f;
  colors[6].z = 0.0f;
  colors[6].w = self->lowHpFlash * self->alpha;
  halfHeight = 3.0f;

  style = UiHpGaugeGetBarStyle(self);
  if (scale == -1.0f) {
    scale = UiHpGaugeGetBarWidth(self);
  }
  if (UiHpGaugeIsLocalPlayer(self) != 0) {
    halfHeight = 7.0f;
  }
  x0 = scale * -0.5f;
  y0 = -halfHeight;
  x1 = scale * 0.5f;
  y1 = halfHeight + 1.0f;
  pxPerHp = scale / self->maxHp;

  /* background sprite, vertices inline and jumped over */
  packed = UiHpGaugePackColor(&colors[5]);
  rect = (GfxGeVertex16 (*)[2])(dl + 2);
  cmd = (u32 *)&rect[1];
  dl[0] = ((PspAddr(cmd) >> 24) & 0xf) << 16 | 0x10000000; /* BASE */
  dl[1] = (PspAddr(cmd) & 0xffffff) | 0x08000000;          /* JUMP */
  ix0 = (int)x0;
  (*rect)[0].x = (s16)ix0;
  iy0 = (int)y0;
  (*rect)[0].y = (s16)iy0;
  (*rect)[0].z = -100;
  ix1 = (int)x1;
  (*rect)[1].x = (s16)ix1;
  iy1 = (int)y1;
  (*rect)[1].y = (s16)iy1;
  (*rect)[1].z = -100;
  cmd[0] = (packed & 0xffffff) | 0x55000000; /* material colour */
  cmd[1] = (packed >> 24) | 0x58000000;      /* material alpha */
  cmd += 2;
  *cmd++ = 0x12000100; /* VTYPE: 16-bit position */
  if (rect != NULL) {
    cmd[0] = ((PspAddr(rect) >> 24) & 0xf) << 16 | 0x10000000; /* BASE */
    cmd[1] = (PspAddr(rect) & 0xffffff) | 0x01000000;          /* VADDR */
    cmd += 2;
  }
  *cmd++ = 0x04060002; /* PRIM sprites, 2 vertices */

  for (segment = 0; segment < 1; segment++) {
    fillX = UiHpGaugeGetSegmentValue(self->hp, self, segment);
    trail = UiHpGaugeGetSegmentValue(self->hpTrail, self, segment);
    fillX = fillX * pxPerHp;
    trail = trail * pxPerHp;
    if (trail <= 0.0f) {
      continue;
    }
    fillX = fillX + x0;
    if (style == 0) {
      spriteColor = colors[style];
      /* the binary also sets f16 = 0.0f, which the sprite emitter does not read */
      cmd = UiHpGaugeEmitBarSprite(x0, y0, fillX, y1, self, cmd, &spriteColor);
    } else {
      segmentColor = colors[style];
      cmd = UiHpGaugeEmitBarSegment(x0, y0, fillX, y1, -30.0f, self, cmd, &segmentColor);
    }
    if (self->hitTimer <= 0.0f) {
      continue;
    }
    /* hit flash: white sprite from the fill end to the trail end */
    colors[4].w = self->hitTimer * self->alpha;
    packed = UiHpGaugePackColor(&colors[4]);
    rect = (GfxGeVertex16 (*)[2])(cmd + 2);
    dl = (u32 *)&rect[1];
    cmd[0] = ((PspAddr(dl) >> 24) & 0xf) << 16 | 0x10000000;
    cmd[1] = (PspAddr(dl) & 0xffffff) | 0x08000000;
    (*rect)[0].x = (s16)(int)fillX;
    (*rect)[0].y = (s16)iy0;
    (*rect)[0].z = -20;
    (*rect)[1].x = (s16)(int)(trail + x0);
    (*rect)[1].y = (s16)iy1;
    (*rect)[1].z = -20;
    dl[0] = (packed & 0xffffff) | 0x55000000;
    dl[1] = (packed >> 24) | 0x58000000;
    dl += 2;
    *dl++ = 0x12000100;
    if (rect != NULL) {
      dl[0] = ((PspAddr(rect) >> 24) & 0xf) << 16 | 0x10000000;
      dl[1] = (PspAddr(rect) & 0xffffff) | 0x01000000;
      dl += 2;
    }
    *dl++ = 0x04060002;
    cmd = dl;
  }

  /* low-HP flash sprite (red) */
  packed = UiHpGaugePackColor(&colors[6]);
  dl = cmd;
  rect = (GfxGeVertex16 (*)[2])(dl + 2);
  cmd = (u32 *)&rect[1];
  dl[0] = ((PspAddr(cmd) >> 24) & 0xf) << 16 | 0x10000000;
  dl[1] = (PspAddr(cmd) & 0xffffff) | 0x08000000;
  (*rect)[0].x = (s16)ix0;
  (*rect)[0].y = (s16)iy0;
  (*rect)[0].z = -10;
  (*rect)[1].x = (s16)ix1;
  (*rect)[1].y = (s16)iy1;
  (*rect)[1].z = -10;
  cmd[0] = (packed & 0xffffff) | 0x55000000;
  cmd[1] = (packed >> 24) | 0x58000000;
  cmd += 2;
  *cmd++ = 0x12000100;
  if (rect != NULL) {
    cmd[0] = ((PspAddr(rect) >> 24) & 0xf) << 16 | 0x10000000;
    cmd[1] = (PspAddr(rect) & 0xffffff) | 0x01000000;
    cmd += 2;
  }
  *cmd++ = 0x04060002;

  /* outline: 9-vertex ABGR4444 line strip, vertices inline and jumped over */
  dl = cmd;
  outline = (GfxGeColor4444Vertex16 (*)[9])(dl + 2);
  cmd = (u32 *)&outline[1];
  shade = (u16)(((int)(self->alpha * 15.0f) & 0xffff) << 12);
  dl[0] = ((PspAddr(cmd) >> 24) & 0xf) << 16 | 0x10000000;
  dl[1] = (PspAddr(cmd) & 0xffffff) | 0x08000000;
  dark = shade | 0x888;
  light = shade | 0xeee;
  midX = (s16)(int)((x0 + x1) * 0.5f);
  midY = (s16)(int)((y0 + y1) * 0.5f);
  (*outline)[0].colour = dark;
  (*outline)[0].x = (s16)ix0;
  (*outline)[0].y = (s16)iy0;
  (*outline)[1].colour = light;
  (*outline)[1].x = midX;
  (*outline)[1].y = (s16)iy0;
  (*outline)[2].colour = dark;
  (*outline)[2].x = (s16)ix1;
  (*outline)[2].y = (s16)iy0;
  (*outline)[3].colour = light;
  (*outline)[3].x = (s16)ix1;
  (*outline)[3].y = midY;
  (*outline)[4].colour = dark;
  (*outline)[4].x = (s16)ix1;
  (*outline)[4].y = (s16)iy1;
  (*outline)[5].colour = light;
  (*outline)[5].x = midX;
  (*outline)[5].y = (s16)iy1;
  (*outline)[6].colour = dark;
  (*outline)[6].x = (s16)ix0;
  (*outline)[6].y = (s16)iy1;
  (*outline)[7].colour = light;
  (*outline)[7].x = (s16)ix0;
  (*outline)[7].y = midY;
  (*outline)[8].colour = (*outline)[0].colour;
  (*outline)[8].x = (*outline)[0].x;
  (*outline)[8].y = (*outline)[0].y;
  for (i = 0; i < 9; i++) {
    (*outline)[i].z = 0;
  }
  cmd[0] = 0x55ffffff; /* material colour white */
  cmd[1] = 0x580000ff; /* material alpha 0xff */
  cmd += 2;
  *cmd++ = 0x12000118; /* VTYPE: colour 4444, 16-bit position */
  if (outline != NULL) {
    cmd[0] = ((PspAddr(outline) >> 24) & 0xf) << 16 | 0x10000000;
    cmd[1] = (PspAddr(outline) & 0xffffff) | 0x01000000;
    cmd += 2;
  }
  *cmd = 0x04020009; /* PRIM line strip, 9 vertices */
  return cmd + 1;
}
