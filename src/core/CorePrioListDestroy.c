// bdc 0x08a305a4 CorePrioListDestroy
#include "bdc.h"

/* Destructor of a `CorePrioList`: frees all entries (`CorePrioListFreeNodes`), releases both
   sentinels (pool or heap), destroys the pool (`CorePrioListFreePool`), clears the cursor and,
   when bit 0 of `flags` is set, frees the list object. */
void CorePrioListDestroy(CorePrioList *list, u32 flags)
{
    if (list == NULL) {
        return;
    }
    CorePrioListFreeNodes(list);
    if (list->pool != NULL) {
        /* Pool-owned sentinels are only destructed (flag 2), not freed. */
        if (MemPoolFree(list->pool, list->active)) {
            CorePrioNodeDelete(list->active, 2);
            list->active = NULL;
        }
        if (MemPoolFree(list->pool, list->pending)) {
            CorePrioNodeDelete(list->pending, 2);
            list->pending = NULL;
        }
    }
    if (list->active != NULL) {
        CorePrioNodeDelete(list->active, 3);
        list->active = NULL;
    }
    if (list->pending != NULL) {
        CorePrioNodeDelete(list->pending, 3);
        list->pending = NULL;
    }
    CorePrioListFreePool(list);
    list->cursor = NULL;
    if (flags & 1) {
        MemLock();
        MemFree(list, NULL, 0);
        MemUnlock();
    }
}
