// bdc 0x089d7784 MemPoolAllocN
#include "bdc.h"

/* Multi-item variant of `MemPoolAlloc`: scans the allocation bitmap of the `MemPool` for the
   first run of `n` consecutive clear bits (first fit, a used item restarts the run; a full word is
   skipped at once), sets those `n` bits and returns the address of the first item
   (`base + itemSize * index`); returns NULL when the pool has no such run. The items are not
   cleared. Give them back with `MemPoolFreeN`. While setting the bits, a bit found already set
   is counted but does not advance the bit position (unreachable for a run found clear). */
void *MemPoolAllocN(MemPool *pool, s32 n)
{
    void *result = NULL;
    s32 start = -1;
    s32 run = 0;
    u32 index = 0;
    u32 bit = 0;
    s32 i;

    if (pool->count >= 1) {
        u32 *word = pool->bitmap;
        u32 bits = *word;

        for (;;) {
            if ((bits & (1u << (bit & 0x1f))) == 0) {
                if (run == 0) {
                    result = (u8 *)pool->base + pool->itemSize * index;
                    start = (s32)index;
                }
                run++;
                if (run >= n) {
                    break;
                }
            } else {
                result = NULL;
                run = 0;
                start = -1;
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
            if ((s32)index >= pool->count) {
                break;
            }
            bits = *word;
        }
    }

    if (result != NULL && run < n) {
        result = NULL;
        start = -1;
    }
    if (result != NULL) {
        s32 wordIndex = start / 32;

        bit = (u32)start & 0x1f;
        for (i = 0; i < n; i++) {
            u32 mask = 1u << (bit & 0x1f);
            u32 bits = pool->bitmap[wordIndex];

            if ((bits & mask) == 0) {
                bit++;
                pool->bitmap[wordIndex] = bits | mask;
                if ((s32)bit > 31) {
                    wordIndex++;
                    bit = 0;
                }
            }
        }
    }
    return result;
}
