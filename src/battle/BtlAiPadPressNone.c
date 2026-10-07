// bdc 0x08899498 BtlAiPadPressNone
#include "bdc.h"

/* Empty pad action of the virtual pad of `BtlAi` (`ai+0x1b0`: two 0x70-byte input
   controllers as read by `BtlInputReadActions`, the current one at `+0` and the previous frame's
   at `+0x70`, owner unit `+0xe0`, stick axes `+0xe4`/`+0xe8`): returns 1 without pressing anything.
   Used by `BtlUnitMode4UpdateRetreat` where a press callback is required. */
s32 BtlAiPadPressNone(void *pad)
{
    (void)pad;
    return 1;
}
