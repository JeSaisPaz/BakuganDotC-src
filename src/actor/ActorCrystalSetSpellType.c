// bdc 0x08859edc ActorCrystalSetSpellType
#include "bdc.h"

/* Writes `type` clamped to 0..9 into the crystal's spell-type table `spellTypes[slot]`, `slot`
   clamped to 0..2. */

void ActorCrystalSetSpellType(ActorCrystal *self, s32 slot, s32 type)

{
  if (slot < 0) {
    slot = 0;
  }
  else if (2 < slot) {
    slot = 2;
  }
  if (type < 0) {
    type = 0;
  }
  else if (9 < type) {
    type = 9;
  }
  self->spellTypes[slot] = type;
}
