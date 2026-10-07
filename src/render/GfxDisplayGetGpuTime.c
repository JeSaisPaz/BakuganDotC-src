// bdc 0x089cec44 GfxDisplayGetGpuTime
#include "bdc.h"

/* Returns the hcounts from the frame start until the GE finish callback fired (`g_gfxGeFinishTime` set
   by `GfxGeFinishCallback` minus `frameStart`). */

s32 GfxDisplayGetGpuTime(GfxDisplay *display)
{
  return g_gfxGeFinishTime - display->frameStart;
}
