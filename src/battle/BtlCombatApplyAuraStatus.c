// bdc 0x088888b0 BtlCombatApplyAuraStatus
#include "bdc.h"

/* Applies status `status` permanently (`BtlCombatApplyStatus` with duration -1). For the
   attribute aura statuses 11..16 that are not yet active it first arms the heal timer
   `regenTimer = 90` (unless it is already running, i.e. > 0), so the first
   `BtlCombatUpdateStatuses` heal comes 90 frames later. */
void BtlCombatApplyAuraStatus(BtlCombatState *combat, s32 status)
{
    if (status >= 11 && status <= 16 && combat->status[status].active == 0 &&
        combat->regenTimer < 1) {
        combat->regenTimer = 90;
    }
    BtlCombatApplyStatus(combat, status, -1);
}
