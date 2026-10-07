// bdc 0x08a2ddd0 SndEmitterListDestroy
#include "bdc.h"

/* Destructor of a `CorePrioList`: frees all entries (`SndEmitterListClear`), releases both
   sentinels (pool or heap), destroys the pool (`SndEmitterListFreePool`), clears the cursor and,
   when bit 0 of `flags` is set, frees the list object. */
void SndEmitterListDestroy(CorePrioList *list, u32 flags)
{
    if (list == NULL) {
        return;
    }
    SndEmitterListClear(list);
    if (list->pool != NULL) {
        /* Pool-owned sentinels are only destructed (flag 2), not freed. */
        if (MemPoolFree(list->pool, list->active)) {
            SndEmitterNodeFree(list->active, 2);
            list->active = NULL;
        }
        if (MemPoolFree(list->pool, list->pending)) {
            SndEmitterNodeFree(list->pending, 2);
            list->pending = NULL;
        }
    }
    if (list->active != NULL) {
        SndEmitterNodeFree(list->active, 3);
        list->active = NULL;
    }
    if (list->pending != NULL) {
        SndEmitterNodeFree(list->pending, 3);
        list->pending = NULL;
    }
    SndEmitterListFreePool(list);
    list->cursor = NULL;
    if (flags & 1) {
        MemLock();
        MemFree(list, NULL, 0);
        MemUnlock();
    }
}
