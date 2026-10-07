// bdc 0x089cea30 GfxEndFrame
#include "bdc.h"

/* Per-frame render end called by `BootMainThread` after the task update/draw:
   `GfxDisplayEndFrame`(`g_gfxDisplay`). */

void GfxEndFrame(void)

{
  GfxDisplayEndFrame(g_gfxDisplay);
  return;
}

