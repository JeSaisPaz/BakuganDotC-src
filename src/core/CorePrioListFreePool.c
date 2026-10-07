// bdc 0x08a30e3c CorePrioListFreePool
#include "bdc.h"

/* Destroys the node pool of a list (`MemPoolDestroy``(pool, 3)`) and clears `pool`. No-op
   without a pool. */
void CorePrioListFreePool(CorePrioList *list)
{
    if (list->pool != NULL) {
        MemPoolDestroy(list->pool, 3);
        list->pool = NULL;
    }
}
