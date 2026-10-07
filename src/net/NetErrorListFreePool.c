// bdc 0x08a3209c NetErrorListFreePool
#include "bdc.h"

/* Destroys the node pool of a net-error list (`MemPoolDestroy(pool, 3)`) and clears `pool`; does
   nothing when there is no pool. Called by `NetErrorListDestroy`. */
void NetErrorListFreePool(CorePrioList *list)
{
    if (list->pool != NULL) {
        MemPoolDestroy(list->pool, 3);
        list->pool = NULL;
    }
}
