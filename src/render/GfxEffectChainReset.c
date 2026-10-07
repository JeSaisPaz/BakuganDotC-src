// bdc 0x089e7fac GfxEffectChainReset
#include "bdc.h"

/* Places every chain point (current and previous) at `pos`, offset upward by 0.0001 per point so
   links have a direction, and zeroes every velocity (bank constant C720). */

void GfxEffectChainReset(GfxEffectChain *chain, const ScePspFVector4 *pos)
{
  ScePspFVector4 start = *pos;
  float y = start.y;
  int i;

  for (i = 0; i < chain->count; i++) {
    chain->points[i] = start;
    chain->points[i].y = y;
    chain->prevPoints[i] = start;
    chain->prevPoints[i].y = y;
    chain->velocities[i].x = 0.0f;
    chain->velocities[i].y = 0.0f;
    chain->velocities[i].z = 0.0f;
    chain->velocities[i].w = 0.0f;
    y = y + 0.0001f;
  }
}
