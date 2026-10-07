// bdc 0x089d749c MemPoolInit
#include "bdc.h"

/* Constructor of a `MemPool`: records `itemSize` and `count`, allocates the item slab
   (`(count*itemSize & ~3) + 4` bytes) and a zero-filled allocation bitmap of `count/32 + 1` words,
   both with `MemAlloc` under `MemLock`. `fromLow` selects allocation from the low end of the
   heap (`MemSetAllocFromLow`) for the duration of each allocation; the previous mode is
   restored. Neither allocation is checked for NULL. Returns `pool`. */
MemPool *MemPoolInit(MemPool *pool, s32 itemSize, s32 count, bool fromLow)
{
    bool prevFromLow;
    size_t bitmapSize;

    pool->itemSize = itemSize;
    pool->count = count;
    pool->totalSize = count * itemSize;

    MemLock();
    prevFromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(fromLow);
    pool->base = MemAlloc(((u32)(count * itemSize) & ~3u) + 4 /* PSP: slab bytes rounded down to a word plus one spare word */, NULL, 0);
    MemSetAllocFromLow(prevFromLow);
    MemUnlock();

    bitmapSize = (size_t)(count / 32 + 1) * sizeof(u32);
    MemLock();
    prevFromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(fromLow);
    pool->bitmap = MemAlloc(bitmapSize, NULL, 0);
    MemSetAllocFromLow(prevFromLow);
    MemUnlock();
    memset(pool->bitmap, 0, bitmapSize);
    return pool;
}
