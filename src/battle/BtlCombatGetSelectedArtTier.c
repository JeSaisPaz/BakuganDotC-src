// bdc 0x08889258 BtlCombatGetSelectedArtTier
#include "bdc.h"

/* Returns the tier (`selectedArt % 4`, C remainder, so negative for negative ids) of the selected
   special art, or -1 when none is selected. Used by `BtlAiEvalCondition`. */
s32 BtlCombatGetSelectedArtTier(BtlCombatState *combat)
{
    s32 art = combat->selectedArt;
    if (art == -1) {
        return -1;
    }
    return art % 4;
}
