// bdc 0x08a1eee0 sceGuDisplay
#include "bdc.h"

/* libgu `sceGuDisplay`: points the frame buffer at the display buffer (or at NULL when `state` is 0)
   and records the new on/off state. Returns the previous state. */

s32 sceGuDisplay(s32 state)
{
  s32 prev;
  void *topaddr;
  s32 bufferwidth;

  if (state == 0) {
    topaddr = (void *)0;
    bufferwidth = 0;
  } else {
    topaddr = (void *)(g_guEdramBase + g_guDispBufOffset);
    bufferwidth = g_guDispBufWidth;
  }
  sceDisplaySetFrameBuf(topaddr, bufferwidth, g_guPixelFormat, 1);
  prev = g_guDisplayOn;
  g_guDisplayOn = state;
  return prev;
}
