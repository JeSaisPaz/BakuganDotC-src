// bdc 0x089ffbd0 CoreBufQueueDtor
#include "bdc.h"

/* Destructor of `CoreBufQueue` (vtable slot 1): reinstalls `g_coreBufQueueVtbl`, deletes the
   local heap (`MemLocalHeapDtor`), frees the data area and the slot array, and frees the queue
   itself when bit 0 of `flags` is set. A NULL `queue` does nothing. */
void CoreBufQueueDtor(CoreBufQueue *queue, u32 flags)
{
    if (queue == NULL) {
        return;
    }
    queue->vtbl = g_coreBufQueueVtbl;
    if (queue->heap != NULL) {
        MemLocalHeapDtor(queue->heap, 3);
        queue->heap = NULL;
    }
    if (queue->data != NULL) {
        void *data = queue->data;

        MemLock();
        MemFree(data, NULL, 0);
        MemUnlock();
        queue->data = NULL;
    }
    if (queue->slots != NULL) {
        void **slots = queue->slots;

        MemLock();
        MemFree(slots, NULL, 0);
        MemUnlock();
        queue->slots = NULL;
    }
    if (flags & 1) {
        MemLock();
        MemFree(queue, NULL, 0);
        MemUnlock();
    }
}
