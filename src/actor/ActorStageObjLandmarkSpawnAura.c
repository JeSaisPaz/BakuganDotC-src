// bdc 0x088a19bc ActorStageObjLandmarkSpawnAura
#include "bdc.h"

/* Spawns effect 0x61 on the effect manager `0x08abd5b0` at the landmark's orb position `+0x390`
   (`GfxEffectSpawnWithOwner`) and makes it follow that position (`effect+0x160`). Called by
   `ActorStageObjLandmarkCtor`; the broken-state variant is
   `ActorStageObjLandmarkSpawnBrokenAura`. */

void ActorStageObjLandmarkSpawnAura(ActorStageObjLandmark *self)

{
  GfxEffect *effect;

  effect = GfxEffectSpawnWithOwner(g_btlUnitEffectMgr,0x61,self->orbPos,self);
  effect->attachPos = self->orbPos;
  return;
}

