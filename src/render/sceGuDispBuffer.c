// bdc 0x08a1ef50 sceGuDispBuffer
#include "bdc.h"

/* libgu `sceGuDispBuffer`: sets the display buffer mode, then copies the pixel format and display
   size into each of the five library contexts. */

void sceGuDispBuffer(s32 width, s32 height, s32 dispbp, s32 dispbw)
{
  s32 i;

  GfxGuSetDispBufferMode(g_guPixelFormat, width, height, dispbp, dispbw);
  for (i = 0; i < 5; i++) {
    g_guContexts[i].psm = g_guPixelFormat;
    g_guContexts[i].dispWidth = g_guDispWidth;
    g_guContexts[i].dispHeight = g_guDispHeight;
  }
}
