// bdc 0x089d771c MemPoolFree
#include "bdc.h"

/* Returns `ptr` to the `MemPool`: computes the item index `(ptr - base) / itemSize` (unsigned)
   and, if it is not above `count`, clears its bit in the allocation bitmap and returns true;
   otherwise returns false to tell the caller that the pointer does not belong to the pool and
   must be freed on the heap. */
bool MemPoolFree(MemPool *pool, void *ptr)
{
    u32 index = (u32)((u8 *)ptr - (u8 *)pool->base) / (u32)pool->itemSize;
    u32 *word = pool->bitmap;

    if (index > (u32)pool->count) {
        return false;
    }
    while (index >= 32) {
        word++;
        index -= 32;
    }
    *word &= ~(1u << (index & 0x1f));
    return true;
}
