// bdc 0x088a2470 ActorStageObjStartEffect
#include "bdc.h"

/* Starts the looping effect of a stage-object subclass if none runs: when `obj+0x328` is NULL it
   spawns effect id 99 attached to the position at `obj+0x390` with `GfxEffectSpawnAttached`
   (manager `g_btlUnitEffectMgr`), stores the new effect in `obj+0x328` and sets effect fields `+0x1d0 =
   1.7f` (0x3fd9999a) and `+0xd8 = 7`. */

void ActorStageObjStartEffect(void *obj)

{
  ActorStageObjLandmark *self = (ActorStageObjLandmark *)obj;

  if (self->effect == NULL) {
    self->effect = GfxEffectSpawnAttached(g_btlUnitEffectMgr,99,self->orbPos);
    self->effect->vec1d0[0] = 1.7f;
    self->effect->textureSlot = 7;
  }
  return;
}
