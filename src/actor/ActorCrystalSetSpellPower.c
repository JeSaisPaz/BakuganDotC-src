// bdc 0x08859f4c ActorCrystalSetSpellPower
#include "bdc.h"

/* Writes `power` clamped to 0.0..2000.0 into the crystal's spell-power table `spellPower[slot]`
   (`+0x9fc`), `slot` clamped to 0..2. */

void ActorCrystalSetSpellPower(ActorCrystal *self, s32 slot, float power)
{
    if (slot < 0) {
        slot = 0;
    } else if (2 < slot) {
        slot = 2;
    }
    if (power < 0.0f) {
        power = 0.0f;
    } else if (!(power <= 2000.0f)) {
        power = 2000.0f;
    }
    self->spellPower[slot] = power;
}
