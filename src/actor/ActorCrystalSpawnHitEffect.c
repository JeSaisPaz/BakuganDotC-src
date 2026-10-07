// bdc 0x088597f4 ActorCrystalSpawnHitEffect
#include "bdc.h"

/* Spawns hit effect 0x34 (`GfxEffectSpawnDirected`) on the unit effect set with the crystal's
   colour index `+0x938`; called by `ActorCrystalOnHit`. */

void ActorCrystalSpawnHitEffect(ActorCrystal *self, float *pos, float *dir)
{
  GfxEffect *fx = GfxEffectSpawnDirected(g_btlUnitEffectMgr, 0x34, pos, dir);
  fx->textureSlot = self->style;
}
