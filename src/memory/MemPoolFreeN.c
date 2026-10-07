// bdc 0x089d78dc MemPoolFreeN
#include "bdc.h"

/* Returns `n` consecutive items starting at `ptr` to the `MemPool`: if `(ptr - base) / itemSize +
   n <= count` (unsigned) it frees them one by one with `MemPoolFree`, stepping `ptr` by
   `itemSize`, and returns true; it returns false when the range does not lie inside the pool (the
   caller must then free the block on the heap) or when `MemPoolFree` rejects an item (items
   before it stay freed). Also returns false for `n <= 0`. */
bool MemPoolFreeN(MemPool *pool, void *ptr, s32 n)
{
    bool freed = false;
    u8 *item = ptr;

    if ((u32)(item - (u8 *)pool->base) / (u32)pool->itemSize + n > (u32)pool->count) {
        return false;
    }
    for (; n > 0; n--) {
        if (!MemPoolFree(pool, item)) {
            return false;
        }
        freed = true;
        item += pool->itemSize;
    }
    return freed;
}
