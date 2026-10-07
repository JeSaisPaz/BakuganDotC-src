// bdc 0x089bca94 CorePowerRequestResume
#include "bdc.h"

/* Under the "COPower" lock sets `manualResume = 1` on a power state that was put into a manual
   suspend by `CorePowerRequestSuspend`; the next `CorePowerUpdate` then returns the machine to
   state 0 (when no system-utility dialog is busy) and clears both manual flags. */
void CorePowerRequestResume(CorePower *power)
{
    CoreLockAcquire(g_corePowerMgr->lock);
    power->manualResume = 1;
    CoreLockRelease(g_corePowerMgr->lock);
}
