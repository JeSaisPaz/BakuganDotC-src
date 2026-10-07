// bdc 0x08888b80 BtlCombatHasArtBelowCharge
#include "bdc.h"

/* Returns 1 when any equipped special-art slot of a `BtlCombatState` (`artIds[i] != -1`) has a
   charge `artCharge[i] / 10000` (computed as `* 0.0001f`) below `ratio`, else 0. Used by the HUD
   advice trigger `BtlHudAdviceTrigger20`. */
s32 BtlCombatHasArtBelowCharge(float ratio, BtlCombatState *combat)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (combat->artIds[i] != -1 && combat->artCharge[i] * 0.0001f < ratio) {
            return 1;
        }
    }
    return 0;
}
