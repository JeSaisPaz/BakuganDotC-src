// bdc 0x088892a4 BtlCombatGetSelectedArtMotion
#include "bdc.h"

/* Returns the attack/motion-set index of the selected special art: `0x3c + tier` (tier = id % 4,
   signed), or 0 when none is selected. `BtlBakuganState17Update` stores it on the unit and
   indexes its motion-set table with it. */
s32 BtlCombatGetSelectedArtMotion(BtlCombatState *combat)
{
    s32 art = combat->selectedArt;
    if (art == -1) {
        return 0;
    }
    return art % 4 + 0x3c;
}
