// bdc 0x088892dc BtlCombatFindArtSlotByTier
#include "bdc.h"

/* Returns the index (0 or 1) of the first of the two selectable art slots whose art id
   `artIds[i]` (not -1) equals `(owner kind - 1) * 4 + tier`, where the owner kind is the
   `unk08` word of the owning unit's object header; -1 when none matches or there is no owner.
   Only slots 0 and 1 are searched. */
s32 BtlCombatFindArtSlotByTier(BtlCombatState *combat, s32 tier)
{
    s32 slot;

    for (slot = 0; slot < 2; slot++) {
        s32 artId = combat->artIds[slot];

        if (artId != -1 && combat->owner != NULL) {
            s32 kind = (s32)((BtlBakugan *)combat->owner)->base.base.unk08;

            if (tier == artId - kind * 4 + 4) {
                return slot;
            }
        }
    }
    return -1;
}
