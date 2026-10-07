// bdc 0x089bc7fc CorePowerGet
#include "bdc.h"

/* Returns the `CorePower` state of the power service (`g_corePowerMgr->power`). No NULL check:
   call `CorePowerIsInitialized` first when the service may not exist yet. Most per-frame power
   functions (`CorePowerUpdate`, `CorePowerIsRunning`, `CorePowerIsSuspended`, ...) take this
   pointer as their argument. */
CorePower *CorePowerGet(void)
{
    return g_corePowerMgr->power;
}
