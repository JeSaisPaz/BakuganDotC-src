// bdc 0x089d7b18 MemInit
#include "bdc.h"

#define MEM_SRC "c:/bullets/bkn2pspsys/src/pspsys/sys/Mem/FOMemMng.cpp"

/* Creates the game heap: destroys any previous one (`MemShutdown`), allocates a 16-byte-aligned
   buffer of `size` (rounded up to 16) and lays out a `MemMng` header (0x40 bytes) plus one free
   `MemBlock` covering the rest, switches allocation to the low end (`MemSetAllocFromLow`), then
   creates `g_memMutex` (`MemCreateMutex`). Result stored in `g_memMng`. Only the `pool` store
   is guarded against a failed `memalign`; the rest writes through the NULL pointer. */
void MemInit(u32 size)
{
    MemMng *mng;
    MemBlock *blk;

    if (g_memMng != NULL) {
        MemShutdown();
    }
    if ((size & 0xf) != 0) {
        size = size - (size & 0xf) + 0x10;
    }
    mng = memalign(0x10, size);
    g_memMng = mng;
    if (mng != NULL) {
        mng->pool = (MemBlock *)(mng + 1);
    }
    mng->totalSize = size;
    mng->usedSize = 0;
    mng->freeSize = mng->totalSize - 0x40;
    mng->blockCount = 0;
    MemSetAllocFromLow(true);
    g_memMng->unk18 = 0;
    /* The list headers are block-shaped sentinels; the init sets their cursor slot, cleared below. */
    MemBlockInit((MemBlock *)&g_memMng->usedList, 0, MEM_SRC, 0x205);
    MemBlockInit((MemBlock *)&g_memMng->freeList, 0, MEM_SRC, 0x206);
    blk = g_memMng->pool;
    g_memMng->usedList.cursor = NULL;
    g_memMng->freeList.cursor = NULL;
    MemBlockInit(blk, g_memMng->freeSize - 0x10, MEM_SRC, 0x20f);
    MemListInsert(&g_memMng->freeList, blk, true);
    g_memMng->freeSize -= 0x10;
    g_memMng->blockCount++;
    MemCreateMutex();
}
