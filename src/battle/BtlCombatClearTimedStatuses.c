// bdc 0x088865b0 BtlCombatClearTimedStatuses
#include "bdc.h"

/* Clears every status slot of a `BtlCombatState` whose `remaining` is not -1 (21 slots,
   `BtlCombatClearStatus` each), so slots marked permanent with `remaining == -1` survive. Run by
   `BtlCombatInit` and `BtlCombatReset`. */
void BtlCombatClearTimedStatuses(BtlCombatState *combat)
{
    int id;

    for (id = 0; id < 21; id++) {
        if (combat->status[id].remaining != -1) {
            BtlCombatClearStatus(combat, id);
        }
    }
}
