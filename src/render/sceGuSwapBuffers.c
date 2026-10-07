// bdc 0x08a1ea00 sceGuSwapBuffers
#include "bdc.h"

u32 sceGuSwapBuffers(void)
{
  s32 newDisp;
  s32 oldDisp;

  if (g_guSwapPreHook != 0) {
    ((void (*)(void))g_guSwapPreHook)();
  }
  newDisp = g_guDrawBufOffset;
  oldDisp = g_guDispBufOffset;
  g_guDispBufOffset = newDisp;
  g_guDrawBufOffset = oldDisp;
  if (g_guDisplayOn == 1) {
    sceDisplaySetFrameBuf(g_guEdramBase + newDisp, g_guDispBufWidth, g_guPixelFormat, 0);
  }
  if (g_guSwapPostHook != 0) {
    ((void (*)(void))g_guSwapPostHook)();
  }
  if (g_guSwapInfoHook != 0) {
    ((void (*)(s32, s32, s32))g_guSwapInfoHook)(g_guPixelFormat, g_guDrawBufOffset, g_guDispBufWidth);
  }
  return g_guDrawBufOffset;
}
