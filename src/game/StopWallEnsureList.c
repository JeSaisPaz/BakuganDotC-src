// bdc 0x088b33f8 StopWallEnsureList
#include "bdc.h"

/* Allocates the 12-byte stop-wall list holder `g_stopWallList` (head, tail, count, all
   zeroed) from the low end of the game heap when it does not exist yet, restoring the previous
   allocation direction afterwards. The allocation result is not checked for NULL. */
void StopWallEnsureList(void)
{
    bool fromLow;
    CoreObjectList *list;

    if (g_stopWallList != NULL) {
        return;
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    list = MemAlloc(sizeof(CoreObjectList), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    g_stopWallList = list;
    list->tail = NULL;
    g_stopWallList->head = NULL;
    g_stopWallList->count = 0;
}
