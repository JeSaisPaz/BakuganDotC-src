// bdc 0x088a2b2c ActorStageObjLandmarkSpawnHitEffect
#include "bdc.h"

/* Spawns the directional hit effect 0x34 on the effect manager `0x08abd5b0`
   (`GfxEffectSpawnDirected(mgr, 0x34, pos, dir)`) with scale 10. */

void ActorStageObjLandmarkSpawnHitEffect(ActorStageObjLandmark *self, const float *pos, const float *dir)

{
  GfxEffect *effect;
  
  effect = GfxEffectSpawnDirected(g_btlUnitEffectMgr,0x34,(float *)pos,(float *)dir);
  effect->size[3] = 0.0f;
  effect->size[0] = 10.0f;
  effect->size[1] = 10.0f;
  effect->size[2] = 10.0f;
  return;
}

