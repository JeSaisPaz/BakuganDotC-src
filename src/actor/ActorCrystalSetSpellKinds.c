// bdc 0x08859910 ActorCrystalSetSpellKinds
#include "bdc.h"

/* Stores the crystal's three spell kinds at `+0x9a0/0x9a4/0x9a8` and recounts the non-zero ones
   into `+0xa30` (the `+0xa2c` counter and the arming fields are left alone, unlike
   `ActorCrystalCastSpells`). */

void ActorCrystalSetSpellKinds(ActorCrystal *self, s32 a, s32 b, s32 c)
{
  s32 count;

  self->spellKinds[0] = a;
  self->spellKinds[1] = b;
  self->spellKinds[2] = c;
  count = (a != 0);
  if (b != 0) {
    count = count + 1;
  }
  if (c != 0) {
    count = count + 1;
  }
  self->spellCount = count;
}
