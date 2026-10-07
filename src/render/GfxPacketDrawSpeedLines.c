// bdc 0x089f31a4 GfxPacketDrawSpeedLines
#include "bdc.h"

/* Draws manga-style focus/speed lines into a render packet. It opens a chunk
   (`GfxPacketBeginChunk`), calls the 2D state list (`GfxDlCall2DState`), writes the screen camera
   (`g_gfxScreenCamera`, `GfxCameraDlWrite` all flags), additive blend with `g_colorWhite`
   (`GfxDlSetBlendState`), and a world matrix translating to (`x`, `y`). The vertices sit inline in
   the chunk behind a JUMP over 256 fans' worth of space. For each of 256 steps it draws a
   random value `r` in [0,1) and, if `r` >= 0.5 and the point (x + cos·w, y + sin·h) of the angle
   set by the last step that passed this test (0 before the first) is on screen (0..480, 0..272), it sets the angle to
   i·2π/256, scales the ellipse `w`×`h` by `1 + spread·r2 - 0.2·spread` (another random `r2`,
   `spread = (400 - min((480-w)·0.7, (272-h)·0.7))·0.004`) and, if that tip is on screen too, emits a
   6-vertex triangle fan: 1.5× the tip, the far points (direction of (w, h) normalised ×768,
   scaled by cos/sin) at the angles i-1, i, i+1, the tip, and i-1 again. The 1.5× vertex and the
   far vertex at angle i use `colour` with alpha `amount·(r-0.3)·1.42857`, the others alpha 0. */

void GfxPacketDrawSpeedLines(float x, float y, float w, float h, float amount, void *packet, const float *colour)
{
  float col[4];
  union { float f[16]; u32 u[16]; } mat;
  float widthR;
  float minR;
  float spread;
  float jitter;
  float dot;
  float inv;
  float outerX;
  float outerY;
  float rnd;
  float rnd2;
  float angle;
  float scale;
  float px;
  float py;
  float tipX;
  float tipY;
  float baseX;
  float baseY;
  float farX;
  float farY;
  float prevX;
  float prevY;
  float nextX;
  float nextY;
  u32 outerColour;
  u32 innerColour;
  u32 *list;
  u32 *jump;
  u32 *cmd;
  GfxGeColorVertexF (*verts)[6]; /* one 6-vertex fan per kept line */
  s32 row;
  s32 colIdx;
  s32 lane;
  s32 i;

  widthR = (480.0f - w) * 0.7f;
  minR = (272.0f - h) * 0.7f;
  if (widthR < minR) {
    minR = widthR;
  }
  angle = 0.0f;
  spread = (400.0f - minR) * 0.004f;
  /* (w, h, 0) normalised (zero length: scaled by 0), each lane clamped to [-1, 1], then ×768.
     The original also scales it by len(w, h) + 32 into two stack temps that are never read. */
  dot = w * w + h * h + 0.0f * 0.0f;
  inv = VfRsq(dot);
  if (dot == 0.0f) {
    inv = 0.0f;
  }
  outerX = VfSat1(w * inv) * 768.0f;
  outerY = VfSat1(h * inv) * 768.0f;
  col[0] = colour[0];
  col[1] = colour[1];
  col[2] = colour[2];
  col[3] = 0.0f;
  /* outer colour: RGBA8 of `colour` with alpha 0 */
  outerColour = 0;
  for (lane = 0; lane < 4; lane++) {
    outerColour |= (u32)VfI2uc(VfF2iz(VfSat0(col[lane]) * 255.0f, 23)) << (lane * 8);
  }
  list = GfxPacketBeginChunk(packet);
  list = GfxDlCall2DState(list);
  list = GfxCameraDlWrite(g_gfxScreenCamera, list, 0xffffffff);
  list = GfxDlSetBlendState(list, &g_colorWhite, 0, 1);
  /* world matrix: identity translated to (x, y, 0) */
  for (row = 0; row < 16; row++) {
    mat.f[row] = (row % 5 == 0) ? 1.0f : 0.0f;
  }
  mat.f[12] = x;
  mat.f[13] = y;
  mat.f[14] = 0.0f;
  mat.f[15] = 1.0f;
  list[0] = 0x3a000000; /* WORLDMATRIXNUMBER 0 */
  for (row = 0; row < 4; row++) {
    for (colIdx = 0; colIdx < 3; colIdx++) {
      /* unaligned lwr at byte 1: top byte of the command word, low 24 bits = float >> 8 */
      list[1 + row * 3 + colIdx] = (g_gfxWorldMatrixDataCmd & 0xff000000) | (mat.u[row * 4 + colIdx] >> 8);
    }
  }
  /* BASE + JUMP over the inline vertex area (256 fans of 6 vertices) */
  jump = list + 13;
  verts = (GfxGeColorVertexF (*)[6])(((uintptr_t)(jump + 2) + 0xf) & ~(uintptr_t)0xf);
  cmd = (u32 *)&verts[256];
  jump[0] = (((uintptr_t)cmd >> 0x18) & 0xf) << 0x10 | 0x10000000;
  jump[1] = ((uintptr_t)cmd & 0xffffff) | 0x8000000;
  jitter = spread * 0.2f;
  for (i = 0; i < 0x100; i++) {
    rnd = PlatformRandFloat12() - 1.0f;
    if (rnd < 0.5f) {
      continue;
    }
    /* on-screen test at the last drawn angle */
    px = x + __builtin_cosf(angle) * w;
    if (!(px <= 480.0f)) {
      continue;
    }
    if (px < 0.0f) {
      continue;
    }
    py = y + __builtin_sinf(angle) * h;
    if (!(py <= 272.0f)) {
      continue;
    }
    if (py < 0.0f) {
      continue;
    }
    col[3] = amount * ((rnd - 0.3f) * 1.42857003f);
    /* inner colour: RGBA8 of `colour` with the faded alpha */
    innerColour = 0;
    for (lane = 0; lane < 4; lane++) {
      innerColour |= (u32)VfI2uc(VfF2iz(VfSat0(col[lane]) * 255.0f, 23)) << (lane * 8);
    }
    angle = 0.0245436933f * (float)i;
    rnd2 = PlatformRandFloat12() - 1.0f;
    scale = spread * rnd2 + 1.0f - jitter;
    tipX = __builtin_cosf(angle) * w * scale;
    if (!(x + tipX <= 480.0f)) {
      continue;
    }
    if (x + tipX < 0.0f) {
      continue;
    }
    tipY = __builtin_sinf(angle) * h * scale;
    if (!(y + tipY <= 272.0f)) {
      continue;
    }
    if (y + tipY < 0.0f) {
      continue;
    }
    baseX = __builtin_cosf(angle) * w * scale * 1.5f;
    baseY = __builtin_sinf(angle) * h * scale * 1.5f;
    farX = __builtin_cosf(angle) * outerX;
    farY = __builtin_sinf(angle) * outerY;
    prevX = __builtin_cosf(angle - 0.0245436933f) * outerX;
    prevY = __builtin_sinf(angle - 0.0245436933f) * outerY;
    nextX = __builtin_cosf(angle + 0.0245436933f) * outerX;
    nextY = __builtin_sinf(angle + 0.0245436933f) * outerY;
    (*verts)[0].colour = innerColour;
    (*verts)[0].x = baseX;
    (*verts)[0].y = baseY;
    (*verts)[1].colour = outerColour;
    (*verts)[1].x = prevX;
    (*verts)[1].y = prevY;
    (*verts)[2].colour = innerColour;
    (*verts)[2].x = farX;
    (*verts)[2].y = farY;
    (*verts)[3].colour = outerColour;
    (*verts)[3].x = nextX;
    (*verts)[3].y = nextY;
    (*verts)[4].colour = outerColour;
    (*verts)[4].x = tipX;
    (*verts)[4].y = tipY;
    (*verts)[5].colour = outerColour;
    (*verts)[5].x = prevX;
    (*verts)[5].y = prevY;
    (*verts)[5].z = 0.0f;
    (*verts)[4].z = 0.0f;
    (*verts)[3].z = 0.0f;
    (*verts)[2].z = 0.0f;
    (*verts)[1].z = 0.0f;
    (*verts)[0].z = 0.0f;
    *cmd++ = 0x1200019c; /* VTYPE: colour 8888, float position */
    if (verts != NULL) {
      cmd[0] = (((uintptr_t)verts >> 0x18) & 0xf) << 0x10 | 0x10000000;
      cmd[1] = ((uintptr_t)verts & 0xffffff) | 0x1000000; /* VADDR */
      cmd += 2;
    }
    *cmd++ = 0x4050006; /* PRIM triangle fan, 6 vertices */
    verts++;
  }
  GfxPacketEndChunk(packet, cmd);
}
