// bdc 0x08888700 BtlCombatTickEnergyRegen
#include "bdc.h"

/* Per-frame energy-regeneration tick of a combat state that is not dead and has stats loaded:
   unless `mode` is 1, decrements regenDelay by 1.0 (floored at 0.0) and, while it is <= 0.0
   (false for NaN), applies energy action 0, or action 0xb when `mode` is 2. */
void BtlCombatTickEnergyRegen(BtlCombatState *combat, s32 mode)
{
    if (combat->dead != 0 || combat->stats == NULL) {
        return;
    }
    if (mode == 1) {
        return;
    }
    combat->regenDelay = combat->regenDelay - 1.0f;
    if (combat->regenDelay < 0.0f) {
        combat->regenDelay = 0.0f;
    }
    if (combat->regenDelay <= 0.0f) {
        if (mode == 2) {
            BtlCombatApplyActionEnergy(combat, 0xb, 0);
            return;
        }
        BtlCombatApplyActionEnergy(combat, 0, 0);
    }
}
