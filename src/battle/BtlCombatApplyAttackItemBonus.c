// bdc 0x088893ec BtlCombatApplyAttackItemBonus
#include "bdc.h"

/* Attacker-side boost items on outgoing damage (`BtlCombatApplyItemBonus`, rate 1.0, mode 0):
   item pair 0x11/0x12 for attack ids 0xb5..0xb7 (`BtlIsAttackIdB5ToB7`), else 0xf/0x10 for the
   id set of `BtlIsItem0FBoostAttackId`, else 1/2 for special hits and 3/4 for normal hits; then
   the pinch pair 0x19/0x1a when the attacker's HP ratio is below 0.2 (`BtlCombatGetHpRatio`).
   Returns the boosted damage. */
float BtlCombatApplyAttackItemBonus(float damage, BtlCombatState *attacker, s32 attackId, u8 special)
{
    s32 itemId;
    float value;

    if (BtlIsAttackIdB5ToB7(attacker, attackId) != 0) {
        itemId = 0x11;
    } else if (BtlIsItem0FBoostAttackId(attacker, attackId) != 0) {
        itemId = 0xf;
    } else if (special != 0) {
        itemId = 1;
    } else {
        itemId = 3;
    }
    value = BtlCombatApplyItemBonus(1.0f, damage, attacker, itemId, 0);
    if (BtlCombatGetHpRatio(attacker) < 0.200000003f) { /* 0x3e4ccccd */
        value = BtlCombatApplyItemBonus(1.0f, value, attacker, 0x19, 0);
    }
    return value;
}
