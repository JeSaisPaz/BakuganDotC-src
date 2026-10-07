// bdc 0x089ce834 GfxGeFinishCallback
#include "bdc.h"

/* GE finish callback installed with `sceGuSetCallback`(4, ...) by `GfxDisplaySetup`: records
   the current `sceDisplayGetAccumulatedHcount` in `g_gfxGeFinishTime`, the time at which the GE finished
   the frame's list. `GfxDisplayGetGpuTime` subtracts the frame start from it. */

void GfxGeFinishCallback(void)

{
  g_gfxGeFinishTime = sceDisplayGetAccumulatedHcount();
  return;
}

