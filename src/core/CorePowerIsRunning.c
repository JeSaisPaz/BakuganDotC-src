// bdc 0x089bcae0 CorePowerIsRunning
#include "bdc.h"

/* Returns 1 when the power state machine is in the running state (`power->state == 0`), read under
   the "COPower" lock. `main` only services the memory stick (`CoreMsUpdate`) while this is
   true. */
int CorePowerIsRunning(CorePower *power)
{
    int running;

    CoreLockAcquire(g_corePowerMgr->lock);
    running = power->state == 0;
    CoreLockRelease(g_corePowerMgr->lock);
    return running;
}
