// bdc 0x089bc7d4 CorePowerIsInitialized
#include "bdc.h"

/* Returns 1 when the power service exists: `g_corePowerMgr` is set and its `power` state
   (`CorePower`) was created, else 0. `main` and `BootMainThread` use it as a guard before
   touching the power state. */
int CorePowerIsInitialized(void)
{
    if (g_corePowerMgr != NULL && g_corePowerMgr->power != NULL)
        return 1;
    return 0;
}
