// bdc 0x0882db50 BtlHudUpdateStatusIcons
#include "bdc.h"

/* Unless `unit` is NULL, resets the shown-icon count iconCount and runs
   `BtlHudAnimateStatusIcon` for icons 0..5 with the `active` flags of the unit's combat status
   slots 11..16, counting the active ones in iconCount after each call so the icons pack left to
   right. */

void BtlHudUpdateStatusIcons(BtlHud *self, BtlBakugan *unit)
{
    s32 i;
    bool active;

    if (unit == NULL) {
        return;
    }
    self->iconCount = 0;
    for (i = 0; i < 6; i++) {
        active = unit->combat.status[i + 11].active != 0;
        BtlHudAnimateStatusIcon(self, i, active);
        if (active) {
            self->iconCount = self->iconCount + 1;
        }
    }
}
