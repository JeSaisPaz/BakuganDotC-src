// bdc 0x089e109c GfxSetMotionTimeScale
#include "bdc.h"

/* Sets the global motion time scale `g_gfxMotionTimeScale` (1.0 normally, from `GmoSystemInit`; battle code
   changes it for slow motion / pauses). */

void GfxSetMotionTimeScale(float scale)
{
  g_gfxMotionTimeScale = scale;
}
