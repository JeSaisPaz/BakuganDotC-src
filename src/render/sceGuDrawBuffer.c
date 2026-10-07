// bdc 0x08a1eac8 sceGuDrawBuffer
#include "bdc.h"

/* libgu sceGuDrawBuffer: optional hook (psm, fbp, fbw), emits PSM/FBW/FBP,
   copies psm and display size into all 5 contexts, records the draw buffer,
   and defaults the depth buffer right after the frame when unset. */
void sceGuDrawBuffer(s32 psm, u32 fbp, s32 fbw)
{
  u32 *cmd;
  GuContext *ctx;
  s32 i;

  if (g_guSwapInfoHook != 0) {
    ((void (*)(s32, u32, s32))g_guSwapInfoHook)(psm, fbp, fbw);
  }
  cmd = (u32 *)g_guCurrentContext->listCurrent;
  cmd[0] = (u32)psm | 0xd2000000;
  cmd[1] = (((fbp & 0xfffffff) >> 0x18) << 0x10) | 0x9d000000 | (u32)fbw;
  cmd[2] = (fbp & 0xffffff) | 0x9c000000;
  g_guCurrentContext->listCurrent = (unsigned char *)(cmd + 3);

  ctx = g_guContexts;
  for (i = 4; i >= 0; i--) {
    ctx->dispWidth = g_guDispWidth;
    ctx->dispHeight = g_guDispHeight;
    ctx->psm = psm;
    ctx++;
  }

  g_guPixelFormat = psm;
  g_guDrawBufOffset = (s32)fbp;
  g_guDispBufWidth = fbw;
  if (g_guDepthBufPtr == 0 && g_guDispHeight != 0) {
    g_guDepthBufPtr = fbp + (u32)(g_guDispHeight * fbw) * 4;
  }
  if (g_guDepthBufWidth == 0) {
    g_guDepthBufWidth = (u32)fbw;
  }
}
