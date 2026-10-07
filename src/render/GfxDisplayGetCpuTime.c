// bdc 0x089cec18 GfxDisplayGetCpuTime
#include "bdc.h"

/* Returns the hcounts elapsed since the frame start (`sceDisplayGetAccumulatedHcount() -
   frameStart`): the time the CPU needed before waiting for the GE. */

s32 GfxDisplayGetCpuTime(GfxDisplay *display)
{
  s32 now = sceDisplayGetAccumulatedHcount();
  return now - display->frameStart;
}
