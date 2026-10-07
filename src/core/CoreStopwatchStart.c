// bdc 0x089bea28 CoreStopwatchStart
#include "bdc.h"

/* Stores the current local time (`sceRtcGetCurrentClockLocalTime`) in slot `index` of
   `g_coreStopwatchSlots`; indices >= `g_coreStopwatchCount` are ignored. Start half of the
   debug stopwatch pair with `CoreStopwatchLap`. */
void CoreStopwatchStart(s32 index)
{
    if (index < g_coreStopwatchCount) {
        sceRtcGetCurrentClockLocalTime(&g_coreStopwatchSlots[index]);
    }
}
