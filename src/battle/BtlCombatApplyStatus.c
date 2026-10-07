// bdc 0x088887b8 BtlCombatApplyStatus
#include "bdc.h"

/* Applies timed status `id` (0..0x14) for `duration` frames to a `BtlCombatState`: an inactive
   slot is activated with `remaining = duration`; an active slot is only extended when its
   `remaining` is below `duration`. Whenever `remaining` was written, `total` (the denominator of
   `BtlCombatGetStatusRatio`) is set to it. Newly applying status 0x12 arms `status12Timer = 30`
   and status 0x14 arms `status14Timer = 15`, the damage-tick countdowns of
   `BtlCombatUpdateStatuses`. */
void BtlCombatApplyStatus(BtlCombatState *combat, int id, int duration)
{
    bool written = false;

    if (id == 0x12 && !combat->status[0x12].active) {
        combat->status12Timer = 30;
    }
    if (id == 0x14 && !combat->status[0x14].active) {
        combat->status14Timer = 15;
    }
    if (!combat->status[id].active) {
        combat->status[id].active = 1;
        combat->status[id].remaining = (s16)duration;
        written = true;
    } else if (combat->status[id].remaining < duration) {
        combat->status[id].remaining = (s16)duration;
        written = true;
    }
    if (written) {
        combat->status[id].total = combat->status[id].remaining;
    }
}
