// bdc 0x08a2e8bc NetAdhocListDestroy
#include "bdc.h"

/* Destructor of the ad-hoc manager's `CoreList` copy (`NetAdhocListInit`): does nothing for
   NULL; empties the list (`NetAdhocListClear`), returns the sentinel to the pool (`MemPoolFree`;
   a non-zero result means it lay inside the pool and `sentinel` is cleared) or frees it on the
   heap, destroys the pool (`MemPoolDestroy(pool, 3)`), and when bit 0 of `flags` is set frees the
   list object itself (`MemFree` under `MemLock`). */
void NetAdhocListDestroy(CoreList *list, u32 flags)
{
    if (list == NULL) {
        return;
    }
    NetAdhocListClear(list);
    if (list->pool != NULL) {
        if (MemPoolFree(list->pool, list->sentinel)) {
            list->sentinel = NULL;
        }
        if (list->pool != NULL) {
            MemPoolDestroy(list->pool, 3);
            list->pool = NULL;
        }
    }
    if (list->sentinel != NULL) {
        CoreListNode *sentinel = list->sentinel;

        MemLock();
        MemFree(sentinel, NULL, 0);
        MemUnlock();
        list->sentinel = NULL;
    }
    if (flags & 1) {
        MemLock();
        MemFree(list, NULL, 0);
        MemUnlock();
    }
}
