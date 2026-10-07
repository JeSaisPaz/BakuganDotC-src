// bdc 0x0888742c BtlGetAttributeMultiplier
#include "bdc.h"

/* Picks the affinity tier and returns `BtlGetAttributeAffinity``(attackerAttr, targetAttr,
   tier)`: tier 3 when `kind == 3` (stage hazard hits), otherwise 1 for `special == 1`, 2 for
   `special == 2` and 0 for any other `special`. */
float BtlGetAttributeMultiplier(s32 kind, s32 attackerAttr, s32 special, s32 targetAttr)
{
    s32 tier;

    if (kind == 3) {
        tier = 3;
    } else if (special == 1) {
        tier = 1;
    } else if (special == 2) {
        tier = 2;
    } else {
        tier = 0;
    }
    return BtlGetAttributeAffinity(attackerAttr, targetAttr, tier);
}
