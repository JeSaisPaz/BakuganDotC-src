// bdc 0x089bca30 CorePowerIsSuspended
#include "bdc.h"

/* Returns 1 when the power state machine has reached the suspended state (`power->state == 2`:
   suspend callbacks ran and the volatile-memory region was handed back), read under the "COPower"
   lock. `main` combines it with `BootIsExitRequested` to know when it is safe to tear the game
   down. */
int CorePowerIsSuspended(CorePower *power)
{
    int state;

    CoreLockAcquire(g_corePowerMgr->lock);
    state = power->state;
    CoreLockRelease(g_corePowerMgr->lock);
    return state == 2;
}
