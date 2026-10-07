// bdc 0x089bc9e0 CorePowerRequestSuspend
#include "bdc.h"

/* Asks the power state machine for a software-driven suspend cycle: under the "COPower" lock sets
   `manualSuspend = 1` and clears `manualResume`. The next `CorePowerUpdate` then moves the state
   to 1 or 2 (releasing the volatile region through `CorePowerStep`) without waiting for a PSP
   power event; `CorePowerRequestResume` ends it. */
void CorePowerRequestSuspend(CorePower *power)
{
    CoreLockAcquire(g_corePowerMgr->lock);
    power->manualSuspend = 1;
    power->manualResume = 0;
    CoreLockRelease(g_corePowerMgr->lock);
}
