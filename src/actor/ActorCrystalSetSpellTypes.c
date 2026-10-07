// bdc 0x08859944 ActorCrystalSetSpellTypes
#include "bdc.h"

/* Writes the crystal's three spell types to `+0x9ac/0x9b0/0x9b4` unclamped (one entry at a time
   with a 0..9 clamp: `ActorCrystalSetSpellType`). */

void ActorCrystalSetSpellTypes(ActorCrystal *self, u32 a, u32 b, u32 c)
{
  self->spellTypes[0] = a;
  self->spellTypes[1] = b;
  self->spellTypes[2] = c;
}
