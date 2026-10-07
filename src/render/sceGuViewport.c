// bdc 0x08a1f0ec sceGuViewport
#include "bdc.h"

/* libgu `sceGuViewport`: emits the four viewport commands (0x42/0x43 scale, 0x45/0x46 centre) with
   the top 24 bits of each float's bit pattern as the argument. */

typedef union GuFloatBits {
  float f;
  u32 u;
} GuFloatBits;

void sceGuViewport(s32 cx, s32 cy, s32 width, s32 height)
{
  GuContext *ctx;
  GuFloatBits w, h, x, y;
  u32 *p;

  w.f = (float)width * g_guViewportScaleX;
  h.f = (float)height * g_guViewportScaleY;
  x.f = (float)cx;
  y.f = (float)cy;
  ctx = g_guCurrentContext;
  p = (u32 *)ctx->listCurrent;
  p[0] = (w.u >> 8) | 0x42000000;
  ctx->listCurrent = (u8 *)(p + 4);
  p[1] = (h.u >> 8) | 0x43000000;
  p[2] = (x.u >> 8) | 0x45000000;
  p[3] = (y.u >> 8) | 0x46000000;
}
