// bdc 0x089d7fa8 MemFree
#include "bdc.h"

/* Returns a block obtained from `MemAlloc` to the game heap `g_memMng` (original
   `FOMemMng_Free`) and coalesces it with physically adjacent free blocks (first the next block in
   the free list, then the previous one). Returns true on success, false when there is no heap or
   `ptr` is NULL (the `MemListRemove` result is tested but it only fails for a NULL block; list
   membership is not checked). A header whose `data` tag does not match
   `ptr` prints an error twice and hits a `break` trap. Not locked internally; callers hold
   `MemLock`. `file`/`line` are unused debug tags (callers pass 0/0). */
bool MemFree(void *ptr, const char *file, s32 line)
{
    MemBlock *blk;
    MemBlock *next;
    MemBlock *prev;

    (void)file;
    (void)line;
    if (g_memMng == NULL || ptr == NULL) {
        return false;
    }
    blk = (MemBlock *)ptr - 1;
    if (blk->data != ptr) {
        sceKernelPrintf(" FOMemMng_Free( 0x%08x )エラー\n", ptr);
        CoreDebugNop();
        sceKernelPrintf(" FOMemMng_Free( 0x%08x )エラー\n", ptr);
        __builtin_trap(); /* break 0x0 */
        return false;
    }
    if (!MemListRemove(&g_memMng->usedList, blk)) {
        return false;
    }
    MemListInsert(&g_memMng->freeList, blk, true);
    g_memMng->freeSize += blk->size;
    g_memMng->usedSize -= blk->size;

    next = blk->next;
    if (next != NULL && (MemBlock *)((u8 *)blk->data + blk->size) == next) {
        blk->size = next->size + blk->size + 0x10;
        MemListRemove(&g_memMng->freeList, next);
        g_memMng->freeSize += 0x10;
        g_memMng->blockCount--;
    }

    prev = blk->prev;
    if (prev != NULL && prev != (MemBlock *)&g_memMng->freeList &&
        (MemBlock *)((u8 *)prev->data + prev->size) == blk) {
        prev->size = blk->size + prev->size + 0x10;
        MemListRemove(&g_memMng->freeList, blk);
        g_memMng->freeSize += 0x10;
        g_memMng->blockCount--;
    }
    return true;
}
