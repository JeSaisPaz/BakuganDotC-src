// bdc 0x08887cfc BtlCombatSetEnergy
#include "bdc.h"

/* Stores the energy gauge value of a `BtlCombatState` clamped to `[0.0, 1000.0]` (a NaN becomes
   1000.0). Ignored while the stat table is NULL. */
void BtlCombatSetEnergy(float energy, BtlCombatState *combat)
{
    if (combat->stats != NULL) {
        if (!(energy <= 1000.0f)) {
            energy = 1000.0f;
        }
        if (energy < 0.0f) {
            energy = 0.0f;
        }
        combat->energy = energy;
    }
}
