// bdc 0x08888a9c BtlCombatIsArtSlotReady
#include "bdc.h"

/* Returns 1 when special-art slot `slot` (clamped to 0..1) of a `BtlCombatState` can be used:
   its art id is not -1, its `listedStatusIds` entry is 0 and its charge is not below 10000.0
   (a NaN charge also passes); otherwise 0. */
int BtlCombatIsArtSlotReady(BtlCombatState *combat, int slot)
{
    if (slot < 0) {
        slot = 0;
    } else if (slot > 1) {
        slot = 1;
    }
    if (combat->artIds[slot] != -1 && combat->listedStatusIds[slot] == 0 &&
        !(combat->artCharge[slot] < 10000.0f)) {
        return 1;
    }
    return 0;
}
