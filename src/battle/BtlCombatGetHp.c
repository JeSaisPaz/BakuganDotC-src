// bdc 0x08887b54 BtlCombatGetHp
#include "bdc.h"

/* Returns the current hit points of a unit's `BtlCombatState`, or 0.0 while its stat table is
   NULL. Counterpart of `BtlCombatGetMaxHp`. */
float BtlCombatGetHp(BtlCombatState *combat)
{
    if (combat->stats != NULL) {
        return combat->hp;
    }
    return 0.0f;
}
