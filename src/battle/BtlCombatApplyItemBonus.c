// bdc 0x08887f24 BtlCombatApplyItemBonus
#include "bdc.h"

/* Applies the boost-item pair (`itemId`, `itemId + 1`) of a `BtlCombatState` to `value`: sums
   `BtlCombatGetItemBonusPercent` for each equipped id (`BtlCombatHasUpgrade`) into the integer
   `points` (truncated after each step). With no points returns `value` unchanged; mode 0 returns
   `(rate * points + 100) * value * 0.01` (boost), mode 1 `(100 - rate * points) * value * 0.01`
   (reduction), and any other mode `1.0 * value * 0.01`. */
float BtlCombatApplyItemBonus(float rate, float value, BtlCombatState *combat, s32 itemId, s32 mode)
{
    s32 points = 0;
    float scaled;

    if (BtlCombatHasUpgrade(combat, itemId)) {
        points = (s32)BtlCombatGetItemBonusPercent(combat, itemId);
    }
    if (BtlCombatHasUpgrade(combat, itemId + 1)) {
        points = (s32)((float)points + BtlCombatGetItemBonusPercent(combat, itemId + 1));
    }
    if (points == 0) {
        return value;
    }
    if (mode == 0) {
        scaled = (rate * (float)points + 100.0f) * value;
    } else if (mode == 1) {
        scaled = (100.0f - rate * (float)points) * value;
    } else {
        scaled = 1.0f * value;
    }
    return scaled * 0.00999999978f;
}
