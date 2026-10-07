// bdc 0x088b4464 ActorStageObjCrystalSpawnHitEffect
#include "bdc.h"

/* Spawns the directional hit effect 0x34 (`GfxEffectSpawnDirected` on `g_btlUnitEffectMgr`) at `pos`
   toward `dir`, tinted with the crystal's element `+0x32c` (effect `textureSlot`), scale 5.0. */

void ActorStageObjCrystalSpawnHitEffect(ActorStageObjCrystal *self, float *pos, float *dir)

{
  GfxEffect *fx = GfxEffectSpawnDirected(g_btlUnitEffectMgr, 0x34, pos, dir);

  fx->textureSlot = self->element;
  fx->size[0] = 5.0f;
  fx->size[1] = 5.0f;
  fx->size[2] = 5.0f;
  fx->size[3] = 0.0f;
}
