// bdc 0x089bfaf4 CoreTaskManagerCreate
#include "bdc.h"

/* Creates the global task manager: allocates its 4-byte object from the low end of the game heap
   (under `MemLock`), runs `CoreTaskManagerInit` on it and stores the result (or NULL on
   allocation failure) in `g_taskManager`. Called early in `BootMainThread`, before the frame
   loop. */
void CoreTaskManagerCreate(void)
{
    bool fromLow;
    void *mgr;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mgr = MemAlloc(4, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mgr != NULL) {
        CoreTaskManagerInit(mgr);
    }
    g_taskManager = mgr;
}
