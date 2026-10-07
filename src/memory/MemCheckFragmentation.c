// bdc 0x089d81a0 MemCheckFragmentation
#include "bdc.h"

/* Bytes to MiB as the game computes it: float conversion of the unsigned size times
   9.53674e-07f (just below 2^-20). */
#define MEM_TO_MB(bytes) ((double)((float)(u32)(bytes) * 9.53674e-07f))

/* Heap fragmentation report: under `MemLock` collects up to 4 free blocks of at least 1 MiB; if
   more than one exists, prints "■メモリー断片化警告！！■" (memory fragmentation
   warning), each such block ("free 0x%08x [ 0x%08x ] %.2f Mb", plus "->分断" when a used block
   lies above it) and the `T=/A=/F=` totals in MiB, and returns true. Returns false otherwise or
   without a heap. The Japanese literals are Shift-JIS in the ELF; they are written in UTF-8
   here. */
bool MemCheckFragmentation(void)
{
    MemBlock *large[4];
    MemBlock *block;
    MemBlock *used;
    s32 count;
    s32 i;
    bool fragmented;

    fragmented = false;
    if (g_memMng == NULL) {
        return false;
    }
    MemLock();
    count = 0;
    for (block = g_memMng->freeList.head; block != NULL && count < 4; block = block->next) {
        if (block->size >= 0x100000) {
            large[count] = block;
            count++;
        }
    }
    if (count >= 2) {
        fragmented = true;
        printf("■メモリー断片化警告！！■\n");
        for (i = 0; i < count; i++) {
            block = large[i];
            printf("free 0x%08x [ 0x%08x ] %.2f Mb\n", block->data, block->size,
                   MEM_TO_MB(block->size));
            for (used = g_memMng->usedList.head; used != NULL; used = used->next) {
                if ((uintptr_t)block->data < (uintptr_t)used->data) {
                    printf("->分断 0x%08x [ 0x%08x ]\n", block->data, block->size);
                    break;
                }
            }
        }
        printf("T=%.1f Mb / A=%.1f Mb / F=%.1f Mb\n", MEM_TO_MB(g_memMng->totalSize),
               MEM_TO_MB(g_memMng->usedSize), MEM_TO_MB(g_memMng->freeSize));
    }
    MemUnlock();
    return fragmented;
}
