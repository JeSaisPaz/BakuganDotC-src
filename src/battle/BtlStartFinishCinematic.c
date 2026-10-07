// bdc 0x0884b548 BtlStartFinishCinematic
#include "bdc.h"

/* Starts the end-of-battle cinematic for `unit`. Does nothing once the battle outcome is decided
   (`g_btlBattleOutcome` != 0), when profile flag 0 is set and the profile has any of the flags
   `0x4880`, when `unit` is NULL, or when the cinematic task (id 0x14a) already exists. Otherwise
   cancels BGM channel 0 (`SndBgmCancelChannel`), fades it out over 1 s (`SndBgmQueueStop`),
   stops the units' loop sounds (`BtlStopAllUnitLoopSounds`), creates the task with
   `BtlFinishTaskStart``(unit, arg, mode)` and, for `mode == 0`, turns the field effects off
   (`BtlSetFieldEffectsActive`). */

void BtlStartFinishCinematic(void *battle, void *unit, int arg, int mode)
{
    if (g_btlBattleOutcome != 0) {
        return;
    }
    if (SaveGetProfileFlag0() != 0 && SaveHasProfile() &&
        SaveProfileHasFlags(SaveGetProfile(), 0x4880)) {
        return;
    }
    if (unit == NULL || CoreTaskExists(0x14a) != 0) {
        return;
    }
    SndBgmCancelChannel(0);
    SndBgmQueueStop(1.0f, 0);
    BtlStopAllUnitLoopSounds(battle, false);
    BtlFinishTaskStart(unit, arg, mode);
    if (mode == 0) {
        BtlSetFieldEffectsActive(false);
    }
}
