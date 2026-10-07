// bdc 0x0883f48c BtlResultGetHpPercent
#include "bdc.h"

/* Rated-result score item 0x14: the player unit's remaining HP as a percentage
   (BtlCombatGetHpRatio() x 100, truncated toward zero); 0 without a player unit. */
int BtlResultGetHpPercent(void *hud)
{
    BtlBakugan *unit;
    int percent = 0;

    unit = (BtlBakugan *)BtlHudGetPlayerBakugan(hud);
    if (unit != NULL) {
        percent = (int)(BtlCombatGetHpRatio(&unit->combat) * 100.0f);
    }
    return percent;
}
