// bdc 0x088a41a0 ActorStageObjEggCrystalSpawnHitEffect
#include "bdc.h"

/* Spawns the directional hit effect 0x34 (`GfxEffectSpawnDirected` on `0x08abd5b0`) tinted with
   the crystal's element `+0x350`, scale 10. */

void ActorStageObjEggCrystalSpawnHitEffect(ActorStageObjEggCrystal *self, float *pos, float *dir)

{
  GfxEffect *effect;

  effect = GfxEffectSpawnDirected(g_btlUnitEffectMgr,0x34,pos,dir);
  effect->textureSlot = self->element;
  effect->size[0] = 10.0f;
  effect->size[1] = 10.0f;
  effect->size[2] = 10.0f;
  effect->size[3] = 0.0f;
  return;
}
