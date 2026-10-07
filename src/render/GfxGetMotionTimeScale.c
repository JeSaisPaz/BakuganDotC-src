// bdc 0x089e1090 GfxGetMotionTimeScale
#include "bdc.h"

/* Returns the global motion time scale `g_gfxMotionTimeScale` (frames per tick multiplier used by
   `GfxModelAdvanceMotion`). */

float GfxGetMotionTimeScale(void)

{
  return g_gfxMotionTimeScale;
}

