// bdc 0x08886f90 BtlCombatReset
#include "bdc.h"

/* Resets a unit's `BtlCombatState` to full health: clears the timed statuses
   (`BtlCombatClearTimedStatuses`) and the `dead` flag, sets `maxHp = hp =
   BtlCombatComputeMaxHp`, the energy gauge to 1000.0, clears the `link` pointer and zeroes
   `regenDelay` and `reservedA8`. */

void BtlCombatReset(BtlCombatState *combat)
{
    float maxHp;

    BtlCombatClearTimedStatuses(combat);
    combat->dead = 0;
    maxHp = BtlCombatComputeMaxHp(combat);
    combat->maxHp = maxHp;
    combat->hp = maxHp;
    combat->link = NULL;
    combat->energy = 1000.0f;
    combat->regenDelay = 0.0f;
    combat->reservedA8 = 0.0f;
}
