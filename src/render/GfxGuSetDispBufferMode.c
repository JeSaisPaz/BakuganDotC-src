// bdc 0x08a1e924 GfxGuSetDispBufferMode
#include "bdc.h"

/* Internal part of `sceGuDispBuffer`: runs the optional hook (`g_guDispBufferHook`), records the
   pixel format, display buffer offset/width and the width/height in the libgu state, sets the
   display mode with `sceDisplaySetMode(0, width, height)` and, when the display is on, re-points
   the frame buffer (`sceDisplaySetFrameBuf`). */

void GfxGuSetDispBufferMode(s32 psm, s32 width, s32 height, s32 dispbp, s32 dispbw)
{
  if (g_guDispBufferHook != 0) {
    ((void (*)(void))g_guDispBufferHook)();
  }
  g_guPixelFormat = psm;
  g_guDispBufWidth = dispbw;
  g_guDispBufOffset = dispbp;
  g_guDispWidth = width;
  g_guDispHeight = height;
  sceDisplaySetMode(0, width, height);
  if (g_guDisplayOn == 1) {
    sceDisplaySetFrameBuf(g_guEdramBase + g_guDispBufOffset, dispbw, psm, 1);
  }
}
