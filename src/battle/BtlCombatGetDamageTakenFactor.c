// bdc 0x08888998 BtlCombatGetDamageTakenFactor
#include "bdc.h"

/* Effective damage-taken multiplier of a `BtlCombatState`: `BtlCombatGetDefenseLevelFactor`,
   x 0.5 while status 2 (defense up) is active, x 1.5 while status 3 (defense down) is active,
   then clamped: below 0 gives 0, anything not <= 100 (including NaN) gives 100. Applied to
   incoming damage by `BtlCombatTakeHit`. */
float BtlCombatGetDamageTakenFactor(BtlCombatState *combat)
{
    float factor = BtlCombatGetDefenseLevelFactor(combat);

    if (combat->status[2].active != 0) {
        factor = factor * 0.5f;
    }
    if (combat->status[3].active != 0) {
        factor = factor * 1.5f;
    }
    if (factor < 0.0f) {
        return 0.0f;
    }
    if (!(factor <= 100.0f)) {
        return 100.0f;
    }
    return factor;
}
