// bdc 0x088598b4 ActorCrystalCastSpells
#include "bdc.h"

/* Sets the crystal's three spell kinds (`+0x9a0/0x9a4/0x9a8`) and `focusCamera` (`+0xa41`), counts
   the non-zero kinds into `+0xa2c` and `+0xa30`, and when at least one is set arms a scripted cast:
   `+0xa39 = 1` (take the spell entries in order), `+0x900 = 10` (step of
   `ActorCrystalUpdateSpellCast`), `+0xa3e = 0`. */

void ActorCrystalCastSpells(ActorCrystal *self, s32 a, s32 b, s32 c, u8 focusCamera)

{
  u32 count;

  self->spellKinds[0] = a;
  self->spellKinds[1] = b;
  self->spellKinds[2] = c;
  self->focusCamera = focusCamera;
  count = (a != 0);
  if (b != 0) {
    count = count + 1;
  }
  if (c != 0) {
    count = count + 1;
  }
  self->spellTotal = count;
  self->spellCount = count;
  if (count != 0) {
    self->scriptedCast = 1;
    self->castStep = 10;
    self->cpuEntryDone = 0;
  }
}
