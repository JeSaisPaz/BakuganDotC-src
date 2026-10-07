// bdc 0x0888656c BtlCombatClearStatus
#include "bdc.h"

/* Clears timed-status slot `id` (0..0x14) of a `BtlCombatState` (`active = 0`, `total = 0`,
   `remaining = 0`) and zeroes every entry of the three-entry `listedStatusIds` array that equals
   `id`. */
void BtlCombatClearStatus(BtlCombatState *combat, int id)
{
    int i;

    combat->status[id].active = 0;
    combat->status[id].total = 0;
    combat->status[id].remaining = 0;
    for (i = 0; i < 3; i++) {
        if (combat->listedStatusIds[i] == id) {
            combat->listedStatusIds[i] = 0;
        }
    }
}
