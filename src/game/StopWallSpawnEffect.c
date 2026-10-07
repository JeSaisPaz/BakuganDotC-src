// bdc 0x088b3848 StopWallSpawnEffect
#include "bdc.h"

/* Spawns one stop-wall effect: effect definition 100 on the stage effect manager `g_btlUnitEffectMgr`,
   attached to the wall centre `wall + 0x30` (`GfxEffectSpawnAttached`), updated once
   (`GfxEffectUpdateNow`) and sized `+0x1d0 = width * 0.005`, `+0x1d4 = height * 0.005`. Returns
   the effect. */

void *StopWallSpawnEffect(float width, float height, float unused, StopWall *self, float *pos, char *texName)

{
  GfxEffect *effect;

  effect = GfxEffectSpawnAttached(g_btlUnitEffectMgr, 100, self->centre);
  GfxEffectUpdateNow(effect);
  effect->vec1d0[0] = width * 0.005f;
  effect->vec1d0[1] = height * 0.005f;
  return effect;
}
