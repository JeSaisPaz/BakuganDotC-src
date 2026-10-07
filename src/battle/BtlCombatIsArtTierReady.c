// bdc 0x08889338 BtlCombatIsArtTierReady
#include "bdc.h"

/* Returns 1 when one of the selectable art slots holds the owner's art of tier `tier`
   (`BtlCombatFindArtSlotByTier` does not return -1) and that slot is ready
   (`BtlCombatIsArtSlotReady` returns nonzero), else 0. Used by the CPU AI command filter
   `BtlAiIsCommandAllowed`. */
s32 BtlCombatIsArtTierReady(BtlCombatState *combat, s32 tier)
{
    s32 slot = BtlCombatFindArtSlotByTier(combat, tier);

    if (slot != -1 && BtlCombatIsArtSlotReady(combat, slot) != 0) {
        return 1;
    }
    return 0;
}
