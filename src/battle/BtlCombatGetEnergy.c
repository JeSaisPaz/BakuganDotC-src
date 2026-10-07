// bdc 0x08887ce0 BtlCombatGetEnergy
#include "bdc.h"

/* Returns the unit's energy gauge value (float at `+0x9c` of `BtlCombatState`, range 0..1000) or
   0.0 when the stat table (`+0x88`) is NULL. The gauge starts at 1000.0 (`BtlCombatReset`,
   `BtlCombatSetup`); the combat update regenerates it in state 0 and drains it for action states
   (`BtlCombatDrainEnergy`). */

float BtlCombatGetEnergy(BtlCombatState *combat)
{
    if (combat->stats != NULL) {
        return combat->energy;
    }
    return 0.0f;
}
