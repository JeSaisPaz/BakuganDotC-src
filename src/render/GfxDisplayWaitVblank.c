// bdc 0x089cf070 GfxDisplayWaitVblank
#include "bdc.h"

/* Paces the frame and flips the buffers: converts `frameSkip` (`+0x1c`; 0, 1, 2, 3 -> 1, 2, 3, 4
   vblanks, anything else 1) to a vblank count, waits that many vblanks with
   `sceDisplayWaitVblankStartMultiCB`, calls `sceGuSwapBuffers` and stores the count in
   ``g_netVblankBudget``. */

void GfxDisplayWaitVblank(GfxDisplay *display)
{
  int vblanks;

  switch (display->frameSkip) {
  case 1: vblanks = 2; break;
  case 2: vblanks = 3; break;
  case 3: vblanks = 4; break;
  default: vblanks = 1; break;
  }
  sceDisplayWaitVblankStartMultiCB(vblanks);
  sceGuSwapBuffers();
  g_netVblankBudget = vblanks;
}
