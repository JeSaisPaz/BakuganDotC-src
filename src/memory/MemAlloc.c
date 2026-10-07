// bdc 0x089d7d44 MemAlloc
#include "bdc.h"

/* Allocates `size` bytes (rounded up to 16) from the game heap `g_memMng` (original
   `FOMemMng_Alloc`) and returns the payload pointer, or NULL. Not thread-safe by itself: callers
   hold `MemLock`. `file`/`line` are debug-allocation tags (callers pass 0/0 in this release
   build) and are only forwarded to the unused parameters of `MemBlockInit`.

   Placement follows `MemIsAllocFromLow`: from low takes the first free block that fits and
   splits the remainder off its top as a new free block; otherwise the last (highest-address)
   fitting block is used and the allocation is carved from its top end, leaving the shrunk block
   in the free list. A block is split only when it is more than `size + 0x10` (header) bytes.
   On failure the heap totals are printed. */
void *MemAlloc(s32 size, const char *file, s32 line)
{
    bool fromLow = MemIsAllocFromLow();
    void *result = NULL;
    MemBlock *blk;
    MemBlock *found = NULL;
    MemBlock *rest;
    MemBlockList *usedList;

    if (g_memMng != NULL && size > 0) {
        blk = g_memMng->freeList.head;
        if ((size & 0xf) != 0) {
            size = size - (size & 0xf) + 0x10;
        }
        if (g_memMng->freeSize < (u32)size) {
            sceKernelPrintf(" FOMemMng_Alloc エラー メモリ不足 0x%08x / 0x%08x\n", size,
                            g_memMng->freeSize);
            blk = NULL;
        }
        for (; blk != NULL; blk = blk->next) {
            if (blk->size < (u32)size) {
                continue;
            }
            found = blk;
            if (fromLow) {
                break;
            }
        }

        if (found != NULL) {
            if ((u32)size + 0x10 < found->size) {
                if (fromLow) {
                    MemListRemove(&g_memMng->freeList, found);
                    rest = (MemBlock *)((u8 *)found->data + size);
                    MemBlockInit(rest, found->size - size - 0x10, file, line);
                    MemListInsert(&g_memMng->freeList, rest, true);
                    found->size = size;
                } else {
                    found->size = found->size - size - 0x10;
                    found = (MemBlock *)((u8 *)found->data + found->size);
                    MemBlockInit(found, size, file, line);
                }
                g_memMng->blockCount++;
                g_memMng->freeSize -= 0x10;
                usedList = &g_memMng->usedList;
            } else {
                MemListRemove(&g_memMng->freeList, found);
                usedList = &g_memMng->usedList;
            }
            MemListInsert(usedList, found, false);
            result = found->data;
            g_memMng->usedSize += found->size;
            g_memMng->freeSize -= found->size;
        }
    }

    if (result == NULL) {
        sceKernelPrintf(" メモリ確保失敗 ( %d Byte )\n", size);
        sceKernelPrintf(" ヒープ総量 %d Byte\n", MemGetTotalSize());
        sceKernelPrintf(" 確保メモリ %d Byte\n", MemGetUsedSize());
        sceKernelPrintf(" メモリ残量 %d Byte\n", MemGetFreeSize());
        CoreDebugNop();
    }
    return result;
}
