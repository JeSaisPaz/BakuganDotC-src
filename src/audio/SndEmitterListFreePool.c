// bdc 0x08a2e670 SndEmitterListFreePool
#include "bdc.h"

/* Destroys the node pool of an emitter list: if `list + 0x10` is non-NULL it runs
   `MemPoolDestroy``(pool, 3)` (releases the slab and the pool header) and clears the pointer; a
   list without a pool is left alone. */
void SndEmitterListFreePool(CorePrioList *list)
{
    if (list->pool != NULL) {
        MemPoolDestroy(list->pool, 3);
        list->pool = NULL;
    }
}
