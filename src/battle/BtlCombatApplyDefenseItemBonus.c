// bdc 0x088894bc BtlCombatApplyDefenseItemBonus
#include "bdc.h"

/* Applies the defender-side boost items to incoming `damage` (`BtlCombatApplyItemBonus`, rate
   0.5, mode 1 = reduction): items 7/8 when `special` is non-zero, else items 9/10; then, when the
   defender's HP ratio (`BtlCombatGetHpRatio`) is below 0.2, the pinch items 0x19/0x1a on the
   result. Returns the reduced damage. `attackId` is unused. */
float BtlCombatApplyDefenseItemBonus(float damage, BtlCombatState *combat, s32 attackId, u8 special)
{
    float value;

    (void)attackId;
    value = BtlCombatApplyItemBonus(0.5f, damage, combat, special != 0 ? 7 : 9, 1);
    if (BtlCombatGetHpRatio(combat) < 0.200000003f) {
        value = BtlCombatApplyItemBonus(0.5f, value, combat, 0x19, 1);
    }
    return value;
}
