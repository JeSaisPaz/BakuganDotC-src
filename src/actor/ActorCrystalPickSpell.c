// bdc 0x088577a4 ActorCrystalPickSpell
#include "bdc.h"

/* Fetches the crystal's next spell parameters (kind, attack type, effect, three ints from floats)
   from its spell tables (`spellKinds`, `spellTypes`, `spellPower`, `spellParamA/B/C`): entry
   `index` when `scriptedCast` is set, else a random one of the `spellCount` entries (nothing more
   when there are none); kind defaults to 0x15. */

void ActorCrystalPickSpell(ActorCrystal *self, int *kind, int *type, int index, int *effect, int *a, int *b, int *c)
{
    u32 pick;

    *kind = 0x15;
    if (self->scriptedCast != 0) {
        pick = (u32)index;
    } else {
        if (self->spellCount == 0)
            return;
        pick = (PlatformRandU32() >> 16) * (u32)self->spellCount >> 16;
    }
    *kind = self->spellKinds[pick];
    *type = self->spellTypes[pick];
    *(float *)effect = self->spellPower[pick];
    *a = (int)self->spellParamA[pick];
    *b = (int)self->spellParamB[pick];
    *c = (int)self->spellParamC[pick];
}
