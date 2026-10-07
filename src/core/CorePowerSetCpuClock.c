// bdc 0x089bb558 CorePowerSetCpuClock
#include "bdc.h"

/* Sets the PSP CPU clock to `mhz` MHz, doing nothing if `scePowerGetCpuClockFrequencyInt()` already
   returns that value. Otherwise it calls `scePowerSetClockFrequency350(mhz, mhz, mhz / 2)`: PLL and
   CPU at `mhz`, bus at half of it. */
void CorePowerSetCpuClock(s32 mhz)
{
    if (scePowerGetCpuClockFrequencyInt() != mhz) {
        scePowerSetClockFrequency350(mhz, mhz, mhz / 2);
    }
}
