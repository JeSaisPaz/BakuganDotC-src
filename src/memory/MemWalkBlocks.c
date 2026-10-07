// bdc 0x089d7c78 MemWalkBlocks
#include "bdc.h"

/* Debug dump of the game heap whose output was compiled out: under `MemLock` it walks the used
   list and the free list of `g_memMng` doing nothing per block. */
void MemWalkBlocks(void)
{
    MemBlock *usedHead;
    MemBlock *freeHead;
    MemBlock *blk;

    if (g_memMng == NULL) {
        return;
    }
    MemLock();
    usedHead = g_memMng->usedList.head;
    freeHead = g_memMng->freeList.head;
    if (usedHead != NULL) {
        for (blk = usedHead->next; blk != NULL; blk = blk->next) {
        }
    }
    if (freeHead != NULL) {
        for (blk = freeHead->next; blk != NULL; blk = blk->next) {
        }
    }
    MemUnlock();
}
