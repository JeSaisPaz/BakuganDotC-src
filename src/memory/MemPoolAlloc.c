// bdc 0x089d767c MemPoolAlloc
#include "bdc.h"

/* Takes the first free item of the `MemPool`: scans the allocation bitmap for the first clear bit
   below `count` (skipping a full word at once), sets it and returns `base + itemSize * index`;
   returns NULL when the pool is full. The item is not cleared. */
void *MemPoolAlloc(MemPool *pool)
{
    u32 *word;
    u32 bits;
    u32 bit = 0;
    s32 index = 0;

    if (pool->count <= 0) {
        return NULL;
    }
    word = pool->bitmap;
    bits = *word;
    for (;;) {
        u32 mask = 1u << (bit & 0x1f);

        if ((bits & mask) == 0) {
            *word = bits | mask;
            return (u8 *)pool->base + pool->itemSize * index;
        }
        bit++;
        if (bits == 0xffffffff) {
            index += 32;
            word++;
            bit = 0;
        } else {
            if ((s32)bit > 31) {
                word++;
                bit = 0;
            }
            index++;
        }
        if (index >= pool->count) {
            return NULL;
        }
        bits = *word;
    }
}
