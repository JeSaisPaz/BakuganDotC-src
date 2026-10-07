// bdc 0x08823e58 GfxEffectAdvanceTick
#include "bdc.h"

/* Increments the global effect update tick `g_gfxEffectTick` without updating anything, so
   effects updated next will not be skipped as 'already updated' (`GfxEffectUpdate`). Called by
   `BtlMainPhaseBattle`. */

void GfxEffectAdvanceTick(void)

{
  g_gfxEffectTick = g_gfxEffectTick + 1;
  return;
}

