// bdc 0x08a31088 NetCharaListDestroy
#include "bdc.h"

/* Destructor of the net-character list: does nothing for NULL; empties it
   (`NetCharaListClear`), returns the sentinel to the pool (clearing `sentinel` when the pool
   took it) and destroys the pool, frees a still-held sentinel on the heap, and frees the list
   object itself when bit 0 of `flags` is set. Called by `NetCharaMgrDestroy`. */
void NetCharaListDestroy(CoreList *list, u32 flags)
{
    if (list == NULL) {
        return;
    }
    NetCharaListClear(list);
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
