// bdc 0x08888130 BtlCombatIsEnergyDepleted
#include "bdc.h"

/* Returns 1 when the `BtlCombatState` has a stat table and its energy gauge
   (`BtlCombatGetEnergy`) is below 1.0, else 0 (also 0 for a NaN gauge). Complement of
   `BtlCombatHasEnergy`. */
int BtlCombatIsEnergyDepleted(BtlCombatState *combat)
{
    if (combat->stats != NULL && BtlCombatGetEnergy(combat) < 1.0f) {
        return 1;
    }
    return 0;
}
