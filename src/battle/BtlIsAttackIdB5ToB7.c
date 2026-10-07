// bdc 0x08889388 BtlIsAttackIdB5ToB7
#include "bdc.h"

/* Returns 1 for attack (hit) ids 0xb5..0xb7, else 0; `combat` is unused.
   `BtlCombatApplyAttackItemBonus` uses it to apply the boost-item pair 0x11/0x12 to these hits. */
s32 BtlIsAttackIdB5ToB7(BtlCombatState *combat, s32 attackId)
{
    (void)combat;
    return attackId > 0xb4 && attackId < 0xb8;
}
