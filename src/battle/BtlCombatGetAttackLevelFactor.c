// bdc 0x088872d0 BtlCombatGetAttackLevelFactor
#include "bdc.h"

/* Returns the attack multiplier of a `BtlCombatState` from its attack level (clamped 0..9) in
   `g_btlAttackLevelFactors` (0.70 .. 1.25). */
float BtlCombatGetAttackLevelFactor(BtlCombatState *combat)
{
    s32 level = combat->attackLevel;
    if (level < 0) {
        level = 0;
    } else if (level > 9) {
        level = 9;
    }
    return g_btlAttackLevelFactors[level];
}
