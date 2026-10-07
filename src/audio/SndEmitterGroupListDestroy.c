// bdc 0x08a2fe20 SndEmitterGroupListDestroy
#include "bdc.h"

/* Destructor of the emitter-group list (`g_soundEmitterGroupList`): does nothing for NULL; frees
   all entries (`SndEmitterGroupListClear`), returns both sentinels to the pool (`MemPoolFree`) or
   deletes them (`SndEmitterGroupNodeDelete`, flags 2 for pool nodes, 3 for heap nodes), destroys
   the node pool (`SndEmitterGroupListFreePool`), clears the cursor and, when bit 0 of `flags` is
   set, frees the list object. */
void SndEmitterGroupListDestroy(CorePrioList *list, u32 flags)
{
    if (list == NULL) {
        return;
    }
    SndEmitterGroupListClear(list);
    if (list->pool != NULL) {
        if (MemPoolFree(list->pool, list->active)) {
            SndEmitterGroupNodeDelete(list->active, 2);
            list->active = NULL;
        }
        if (MemPoolFree(list->pool, list->pending)) {
            SndEmitterGroupNodeDelete(list->pending, 2);
            list->pending = NULL;
        }
    }
    if (list->active != NULL) {
        SndEmitterGroupNodeDelete(list->active, 3);
        list->active = NULL;
    }
    if (list->pending != NULL) {
        SndEmitterGroupNodeDelete(list->pending, 3);
        list->pending = NULL;
    }
    SndEmitterGroupListFreePool(list);
    list->cursor = NULL;
    if (flags & 1) {
        MemLock();
        MemFree(list, NULL, 0);
        MemUnlock();
    }
}
