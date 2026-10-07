// bdc 0x08a316ec NetErrorListDestroy
#include "bdc.h"

/* Destructor of a net-error list: does nothing for NULL; frees all nodes
   (`NetErrorListFreeNodes`), gives the `active` and `pending` head nodes back to the pool when
   they lie in it (then runs their destructor with flag 2) or else deletes them (flag 3),
   destroys the pool (`NetErrorListFreePool`), clears `cursor`, and frees the list object
   itself when bit 0 of `flags` is set. Called by `NetErrorMgrDtor`. */
void NetErrorListDestroy(CorePrioList *list, u32 flags)
{
    if (list == NULL) {
        return;
    }
    NetErrorListFreeNodes(list);
    if (list->pool != NULL) {
        if (MemPoolFree(list->pool, list->active)) {
            NetErrorNodeDelete(list->active, 2);
            list->active = NULL;
        }
        if (MemPoolFree(list->pool, list->pending)) {
            NetErrorNodeDelete(list->pending, 2);
            list->pending = NULL;
        }
    }
    if (list->active != NULL) {
        NetErrorNodeDelete(list->active, 3);
        list->active = NULL;
    }
    if (list->pending != NULL) {
        NetErrorNodeDelete(list->pending, 3);
        list->pending = NULL;
    }
    NetErrorListFreePool(list);
    list->cursor = NULL;
    if (flags & 1) {
        MemLock();
        MemFree(list, NULL, 0);
        MemUnlock();
    }
}
