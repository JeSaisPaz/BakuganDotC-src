// bdc 0x088886bc BtlCombatHasEnergy
#include "bdc.h"

/* Returns 1 when the unit's `BtlCombatState` has a stat table (`+0x88`) and its energy gauge
   (`BtlCombatGetEnergy`) is at least 1.0, else 0. Used as a "can still perform an energy-costing
   action" test (16 callers, mostly the unit state machines around `0x08871xxx`). */

int BtlCombatHasEnergy(BtlCombatState *combat)
{
    if (combat->stats == NULL) {
        return 0;
    }
    if (BtlCombatGetEnergy(combat) < 1.0f) {
        return 0;
    }
    return 1;
}
