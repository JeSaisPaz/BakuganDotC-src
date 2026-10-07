// bdc 0x0885b080 BtlLoadRequestListEnsure
#include "bdc.h"

/* Allocates the 12-byte list holder `g_btlLoadRequests` (head, tail, count) for the load
   requests from the low end of the game heap when it does not exist yet, restoring the previous
   allocation direction afterwards. The allocation result is not checked for NULL. */
void BtlLoadRequestListEnsure(void)
{
    bool fromLow;
    CoreObjectList *list;

    if (g_btlLoadRequests != NULL) {
        return;
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    list = MemAlloc(sizeof(CoreObjectList), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    g_btlLoadRequests = list;
    list->tail = NULL;
    g_btlLoadRequests->head = NULL;
    g_btlLoadRequests->count = 0;
}
