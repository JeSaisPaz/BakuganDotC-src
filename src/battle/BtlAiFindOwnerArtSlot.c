// bdc 0x08893240 BtlAiFindOwnerArtSlot
#include "bdc.h"

/* Returns the art slot (0 or 1) of the owner of `BtlAi` whose art id
   (`BtlCombatState` `artIds[slot]`) is not -1 and equals the owner's art of tier `tier`
   (`(kind - 1) * 4 + tier`, kind = the unit's `CoreObject` word `+0x08`), or -2 when neither
   of the first two slots holds it. Used by `BtlAiRunAttackRules`. */
s32 BtlAiFindOwnerArtSlot(BtlAi *self, s32 tier)
{
    BtlBakugan *owner = self->owner;
    s32 wanted = tier + (s32)owner->base.base.unk08 * 4 - 4;
    u32 slot;

    for (slot = 0; slot < 2; slot = (slot + 1) & 0xff) {
        /* The binary clamps the index to the 3-slot array (slot < 3 ? slot : 0); slot never
           reaches 3 here. */
        s32 art = owner->combat.artIds[slot < 3 ? slot : 0];

        if (art != -1 && art == wanted) {
            return (s32)slot;
        }
    }
    return -2;
}
