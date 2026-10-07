// bdc 0x089be954 CoreRtcGetMicrosecond
#include "bdc.h"

/* Returns the microsecond field of the current local RTC time; used as a cheap seed / jitter
   value (e.g. `BtlMainTaskCtor`, `UiEquipRandomPick`). */
u32 CoreRtcGetMicrosecond(void)
{
    ScePspDateTime now;

    sceRtcGetCurrentClockLocalTime(&now);
    return now.microsecond;
}
