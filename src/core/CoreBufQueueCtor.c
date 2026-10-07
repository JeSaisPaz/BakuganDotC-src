// bdc 0x089ffa54 CoreBufQueueCtor
#include "bdc.h"

/* Constructor of the buffer queue (`CoreBufQueue`, vtable `g_coreBufQueueVtbl`) for `count`
   slots of `elemSize` bytes: allocates the slot pointer array (`count * 4`) and a zeroed data
   area (`count * align16(elemSize + 0x10)`), builds a 16-byte local heap over the data area
   (`MemLocalHeapCtor`, alignment 0x10; NULL if its allocation fails), and sets `capacity` =
   `count`, read/write indices = 0. All allocations come from the low heap end. */

static inline void *AllocLow(u32 size)
{
    bool fromLow;
    void *p;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    p = MemAlloc(size, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    return p;
}

CoreBufQueue *CoreBufQueueCtor(CoreBufQueue *queue, int elemSize, int count)
{
    u32 stride = elemSize + 0x10;
    u32 dataSize;
    MemLocalHeap *heap;

    queue->vtbl = g_coreBufQueueVtbl;
    if (stride & 0xf) {
        stride = stride - (stride & 0xf) + 0x10;
    }
    dataSize = count * stride;

    queue->slots = AllocLow(count * sizeof(void *));
    queue->data = AllocLow(dataSize);
    memset(queue->data, 0, dataSize);

    heap = AllocLow(sizeof(MemLocalHeap));
    if (heap != NULL) {
        MemLocalHeapCtor(heap, queue->data, dataSize, 0x10);
    }
    queue->heap = heap;
    queue->capacity = count;
    queue->readIndex = 0;
    queue->writeIndex = 0;
    return queue;
}
