// bdc 0x0885a030 ActorCrystalSetSpellParamB
#include "bdc.h"

/* Stores `(float)value` (clamped 0..9) in the crystal's spell-parameter table `+0xa14[slot]`
   (`slot` clamped 0..2); `ActorCrystalPickSpell` returns it as its `b` output, which
   `ActorCrystalUpdateSpellCast` copies into the attack object. */

void ActorCrystalSetSpellParamB(ActorCrystal *self, s32 slot, s32 value)

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
  else if (9 < value) {
    value = 9;
  }
  self->spellParamB[slot] = (float)value;
  return;
}

