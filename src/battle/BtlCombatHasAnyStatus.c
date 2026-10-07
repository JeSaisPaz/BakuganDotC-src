// bdc 0x08888928 BtlCombatHasAnyStatus
#include "bdc.h"

/* Returns 1 when any of the 21 status slots of a `BtlCombatState` is active, else 0. Used by
   `BtlAiEvalCondition`. */
s32 BtlCombatHasAnyStatus(BtlCombatState *combat)
{
    int i;

    for (i = 0; i < 21; i++) {
        if (combat->status[i].active != 0) {
            return 1;
        }
    }
    return 0;
}
