// bdc 0x089bd21c CorePowerUnlockVolatileMem
#include "bdc.h"

/* Hands the PSP volatile-memory region back (`sceKernelVolatileMemUnlock(0)`) when
   `CorePower``.volatileLocked` is set, and clears `volatileLocked`, `volatileBase` and
   `volatileSize`. Called by `CorePowerStep` once suspend has started and nothing is allocated
   from the volatile heap any more. */
void CorePowerUnlockVolatileMem(CorePower *power)
{
    if (power->volatileLocked) {
        sceKernelVolatileMemUnlock(0);
        power->volatileBase = NULL;
        power->volatileSize = 0;
        power->volatileLocked = 0;
    }
}
