// bdc 0x08859cec ActorCrystalSetEffectsVisible
#include "bdc.h"

/* Sets the crystal's effects-visible flag `+0xb8` and the alpha (1 or 0) of all effects it owns in
   the unit effect set `g_btlUnitEffectMgr` (`GfxEffectSetAlphaOwned`). */

void ActorCrystalSetEffectsVisible(ActorCrystal *self, bool visible)

{
  (self->base).base.visible = visible;
  if (visible) {
    GfxEffectSetAlphaOwned(1.0f,g_btlUnitEffectMgr,-1,self);
    return;
  }
  GfxEffectSetAlphaOwned(0.0f,g_btlUnitEffectMgr,-1,self);
  return;
}

