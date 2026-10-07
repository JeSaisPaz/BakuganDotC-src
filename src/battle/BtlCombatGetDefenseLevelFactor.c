// bdc 0x08888958 BtlCombatGetDefenseLevelFactor
#include "bdc.h"

/* Returns the damage-taken multiplier of a `BtlCombatState` from its defense level (clamped
   0..9) in `g_btlDefenseLevelFactors` (1.20 at level 0 down to 0.35 at level 9). */
float BtlCombatGetDefenseLevelFactor(BtlCombatState *combat)
{
    int level = combat->defenseLevel;

    if (level < 0) {
        level = 0;
    } else if (level > 9) {
        level = 9;
    }
    return g_btlDefenseLevelFactors[level];
}
