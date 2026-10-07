// bdc 0x089cebf0 GfxDisplayMarkFrameStart
#include "bdc.h"

/* Stores the current `sceDisplayGetAccumulatedHcount` in `GfxDisplay``.frameStart` (`+0x28`), the
   reference for this frame's CPU/GPU timing. */

void GfxDisplayMarkFrameStart(GfxDisplay *display)
{
  display->frameStart = sceDisplayGetAccumulatedHcount();
}
