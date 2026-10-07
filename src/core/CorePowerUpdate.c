// bdc 0x089bd3b4 CorePowerUpdate
#include "bdc.h"

/* Per-frame update of the power state machine of `CorePower`, called by `main`'s service loop
   with `CorePowerGet`. Under the "COPower" lock it turns the latest power-callback flags
   (`g_powerCallbackFlags`) into a state change and then, if `active` is set, lets
   `CorePowerStep` do the volatile-memory work. Automatic mode (`manualSuspend == 0`): `lastFlags`
   is refreshed, and SUSPENDING|STANDBY (0x90000) enters state 1 (suspend pending) unless already 1
   or 2, RESUMING (0x20000) enters state 3 unless already 3 or 4, RESUME_COMPLETE (0x40000) returns
   to state 0 and clears `resumeNotify`. Manual mode (`manualSuspend` set by
   `CorePowerRequestSuspend`): states 0/3/4 jump to 1 (when the volatile region is locked) or 2,
   and once `CorePowerRequestResume` set `manualResume` and no system-utility dialog is busy the
   machine returns to state 0 and both manual flags are cleared.

   Quirk kept from the binary: in automatic mode a set `unk16` returns at once *without* releasing
   the lock. */
void CorePowerUpdate(CorePower *power)
{
    CoreLockAcquire(g_corePowerMgr->lock);

    if (!power->manualSuspend) {
        u32 flags;
        s32 state;

        if (power->unk16) {
            return; /* lock left held, as in the original */
        }
        flags = g_powerCallbackFlags;
        state = power->state;
        power->lastFlags = flags;
        if (flags & 0x90000) {
            if (!(state > 0 && state < 3)) {
                power->state = 1;
            }
        } else if (flags & 0x20000) {
            if (!(state > 2 && state < 5)) {
                power->state = 3;
            }
        } else if (flags & 0x40000) {
            power->state = 0;
            if (power->resumeNotify) {
                power->resumeNotify = 0;
            }
        }
    } else {
        u32 state = (u32)power->state;

        if (state <= 4) {
            if (state == 0 || state == 3 || state == 4) {
                power->state = power->volatileLocked ? 1 : 2;
            } else if (!power->unk16 && power->manualResume) {
                if (!SysUtilIsInit()) {
                    power->state = 0;
                    power->manualResume = 0;
                    power->manualSuspend = 0;
                } else if (!SysUtilIsBusy(SysUtilGetCell())) {
                    power->state = 0;
                    power->manualResume = 0;
                    power->manualSuspend = 0;
                }
            }
        }
    }

    if (power->active) {
        CorePowerStep(power);
    }
    CoreLockRelease(g_corePowerMgr->lock);
}
