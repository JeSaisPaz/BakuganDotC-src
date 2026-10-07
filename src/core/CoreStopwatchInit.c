// bdc 0x089be974 CoreStopwatchInit
#include "bdc.h"

/* Creates the table of wall-clock stopwatches: stores `count` in `g_coreStopwatchCount`, frees
   any previous `g_coreStopwatchSlots` array (under `MemLock`) and allocates `count * 16` bytes
   (one `ScePspDateTime` per slot) from the low end of the heap. `main` calls it with 8 right
   after `CorePowerInit`. The slots are not cleared. */
void CoreStopwatchInit(s32 count)
{
    ScePspDateTime *slots;
    bool fromLow;

    g_coreStopwatchCount = count;
    if (g_coreStopwatchSlots != NULL) {
        MemLock();
        MemFree(g_coreStopwatchSlots, NULL, 0);
        MemUnlock();
        g_coreStopwatchSlots = NULL;
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    slots = MemAlloc(count << 4, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    g_coreStopwatchSlots = slots;
}
