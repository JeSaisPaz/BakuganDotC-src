// bdc 0x08887a50 BtlCombatGetMaxHp
#include "bdc.h"

/* Returns the maximum hit points of a unit's `BtlCombatState` as an integer
   (`(int)combat->maxHp`, the float at `+0x94`), or 0 while the state has no stat table
   (`combat->stats`, `+0x88`, is NULL, i.e. before `BtlCombatSetup`). */

int BtlCombatGetMaxHp(BtlCombatState *combat)
{
    if (combat->stats == NULL) {
        return 0;
    }
    /* trunc.w.s with the 2^31 bias: an unsigned float-to-integer conversion. */
    return (int)(u32)combat->maxHp;
}
