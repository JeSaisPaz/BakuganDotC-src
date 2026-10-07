// bdc 0x089ceefc GfxDisplaySetup
#include "bdc.h"

/* Brings up the GE and display: `sceGuInit`, `sceGuStart`(0, `g_gfxGeListBuf`,
   `g_gfxGeListSize` = 0x1000 bytes), copies the default clear colour (`g_colorBlack`, alpha
   forced to 0) into `clearColor`, writes the default state (`GfxSetupGeState`), `sceGuFinish` and
   `sceGuSync`(0, 0), installs the finish (`GfxGeFinishCallback`) and signal
   (`GfxGeSignalCallback`) callbacks with `sceGuSetCallback`, initialises `unk10 = -1`,
   `signalHook`/`frameHook` = NULL, `frameSkip = 1` (30 fps) and `showLoadMeter = 0`, turns the
   display on (`sceGuDisplay`(1)), waits one vblank and finishes with `GfxDisplayInitVram`. */

void GfxDisplaySetup(GfxDisplay *display)
{
  sceGuInit();
  sceGuStart(0, &g_gfxGeListBuf, g_gfxGeListSize);
  display->clearColor[0] = g_colorBlack.x;
  display->clearColor[1] = g_colorBlack.y;
  display->clearColor[2] = g_colorBlack.z;
  display->clearColor[3] = g_colorBlack.w;
  display->clearColor[3] = 0.0f;
  GfxSetupGeState();
  sceGuFinish();
  sceGuSync(0, 0);
  sceGuSetCallback(4, GfxGeFinishCallback);
  sceGuSetCallback(1, GfxGeSignalCallback);
  display->unk10 = -1;
  display->frameSkip = 1;
  display->showLoadMeter = 0;
  display->signalHook = 0;
  display->frameHook = 0;
  sceGuDisplay(1);
  sceDisplayWaitVblankStartCB();
  GfxDisplayInitVram(display);
}
