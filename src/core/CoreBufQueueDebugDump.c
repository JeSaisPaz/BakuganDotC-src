// bdc 0x089ffcd8 CoreBufQueueDebugDump
#include "bdc.h"

/* Debug method (vtable slot 2) of `CoreBufQueue`: iterates over the entries (empty loop in the
   release build) and walks the local heap's lists (`MemLocalHeapDebugWalk`); has no effect. */
void CoreBufQueueDebugDump(void *queue)
{
    CoreBufQueue *bufQueue = queue;
    int i;

    for (i = CoreBufQueueCount(queue) - 1; i >= 0; i--) {
        /* per-entry debug output compiled out */
    }
    MemLocalHeapDebugWalk(bufQueue->heap);
}
