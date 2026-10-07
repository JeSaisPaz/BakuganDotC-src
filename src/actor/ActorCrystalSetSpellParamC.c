// bdc 0x0885a090 ActorCrystalSetSpellParamC
#include "bdc.h"

/* Stores `(float)value` (clamped 0..3) in the crystal's spell-parameter table `+0xa20[slot]`
   (`slot` clamped 0..2); `ActorCrystalPickSpell` returns it as its `c` output, which
   `ActorCrystalUpdateSpellCast` copies into the attack object. */

void ActorCrystalSetSpellParamC(ActorCrystal *self, s32 slot, s32 value)

{
  if (slot < 0) {
    slot = 0;
  }
  else if (2 < slot) {
    slot = 2;
  }
  if (value < 0) {
    value = 0;
  }
  else if (3 < value) {
    value = 3;
  }
  self->spellParamC[slot] = (float)value;
  return;
}

