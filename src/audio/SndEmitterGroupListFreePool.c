// bdc 0x08a3037c SndEmitterGroupListFreePool
#include "bdc.h"

/* Destroys the node pool of the emitter-group list: if `list->pool` (`+0x10`) is set,
   `MemPoolDestroy(pool, 3)` and clear the pointer. Last step of `SndEmitterGroupListDestroy`. */
void SndEmitterGroupListFreePool(CorePrioList *list)
{
    if (list->pool != NULL) {
        MemPoolDestroy(list->pool, 3);
        list->pool = NULL;
    }
}
