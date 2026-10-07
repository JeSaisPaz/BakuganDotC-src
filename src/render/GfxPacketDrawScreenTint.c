// bdc 0x089f1bb8 GfxPacketDrawScreenTint
#include "bdc.h"

/* Draws a full-screen colour overlay into a render packet: skipped when alpha `rgba[3] < 0.015`.
   Disables depth/test state, uses normal alpha blending, or additive blending with `rgba - 1`
   (saturated) when any channel exceeds 1.0 (over-bright flash), sets the material colour from
   `rgba` (each channel saturated, scaled by 255 and truncated to a byte), binds `texture` (or the feedback texture
   `GfxGetFeedbackTexture`), and draws through-mode sprites: `mode == 1` the eight strips of
   `g_gfxScreenStripVerts` (16 vertices), any other mode the rectangle
   `g_gfxScreenSpriteVerts[mode * 2]` after setting its second vertex's texel V to 0xfa (mode 0)
   or 0x100. */

void GfxPacketDrawScreenTint(void *packet, const float *rgba, s32 mode, void *texture)
{
  ScePspFVector4 col __attribute__((aligned(16)));
  GfxGeTexVertex16 *verts;
  u32 *dl;
  u32 *p;
  u32 addr;
  u32 packed;
  u16 texV;

  if (rgba[3] < 0.015f) {
    return;
  }
  dl = GfxPacketBeginChunk((RenderPacket *)packet);
  dl[0] = 0x1e000001;
  dl[1] = 0x22000000;
  dl[2] = 0x23000000;
  dl[3] = 0x24000000;
  dl[4] = 0x21000001;
  dl[5] = 0x483f8000; /* 1.0f >> 8 */
  dl[6] = 0x493f8000;
  dl[7] = 0x4a000000; /* 0.0f >> 8 */
  dl[8] = 0x4b000000;
  col = *(const ScePspFVector4 *)rgba;
  if (!(col.x <= 1.0f) || !(col.y <= 1.0f) || !(col.z <= 1.0f)) {
    dl[9] = 0xdf0000a2;
    dl[10] = 0xe0000000;
    dl[11] = 0xe1ffffff;
    col.x = col.x - 1.0f;
    col.y = col.y - 1.0f;
    col.z = col.z - 1.0f;
    col.x = VfSat0(col.x);
    col.y = VfSat0(col.y);
    col.z = VfSat0(col.z);
    col.w = VfSat0(col.w);
  } else {
    dl[9] = 0xdf000032;
    dl[10] = 0xe0000000;
    dl[11] = 0xe1000000;
  }
  /* vsat0 + vscl 255 + vf2iz 23 + vi2uc: each channel clamp(floor(sat(c) * 255), 0, 255), red in the low byte */
  packed = (u32)VfI2uc(VfF2iz(VfSat0(col.x) * 255.0f, 23))
         | (u32)VfI2uc(VfF2iz(VfSat0(col.y) * 255.0f, 23)) << 8
         | (u32)VfI2uc(VfF2iz(VfSat0(col.z) * 255.0f, 23)) << 16
         | (u32)VfI2uc(VfF2iz(VfSat0(col.w) * 255.0f, 23)) << 24;
  dl[12] = (packed & 0xffffff) | 0x55000000;
  dl[13] = (packed >> 24) | 0x58000000;
  if (texture == NULL) {
    p = GfxTextureWriteCall(GfxGetFeedbackTexture(), dl + 14, 0);
  } else {
    p = GfxTextureWriteCall(texture, dl + 14, 0);
  }
  if (mode == 1) {
    addr = (u32)(uintptr_t)g_gfxScreenStripVerts;
    p[0] = 0x12800102;
    p[1] = ((addr >> 24) & 0xf) << 16 | 0x10000000;
    p[2] = (addr & 0xffffff) | 0x01000000;
    p[3] = 0x04060010;
    p += 4;
  } else {
    texV = 0x100;
    if (mode == 0) {
      texV = 0xfa;
    }
    verts = &g_gfxScreenSpriteVerts[mode * 2];
    g_gfxScreenSpriteVerts[mode * 2 + 1].v = (s16)texV;
    p[0] = 0x12800102;
    p++;
    if (verts != NULL) {
      addr = (u32)(uintptr_t)verts;
      p[0] = ((addr >> 24) & 0xf) << 16 | 0x10000000;
      p[1] = (addr & 0xffffff) | 0x01000000;
      p += 2;
    }
    p[0] = 0x04060002;
    p++;
  }
  GfxPacketEndChunk((RenderPacket *)packet, p);
}
