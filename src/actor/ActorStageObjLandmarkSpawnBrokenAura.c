// bdc 0x088a3154 ActorStageObjLandmarkSpawnBrokenAura
#include "bdc.h"

/* Spawns effect 0x62 on the effect manager `0x08abd5b0` at the landmark's orb position `+0x390` and
   makes it follow that position. Called from `ActorStageObjLandmarkState01Broken`. */

void ActorStageObjLandmarkSpawnBrokenAura(ActorStageObjLandmark *self)

{
  GfxEffect *effect;

  effect = GfxEffectSpawnWithOwner(g_btlUnitEffectMgr,0x62,self->orbPos,self);
  effect->attachPos = self->orbPos;
  return;
}

