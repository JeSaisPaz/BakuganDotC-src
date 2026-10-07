// bdc 0x08a018c4 MemLocalHeapFree
#include "bdc.h"

/* Frees a block of a `MemLocalHeap`: the header sits right before `ptr` and must carry `ptr` as
   its `data` tag; it is moved from the used to the free list, the counters are updated, and it is
   coalesced with the physically following free block (when its `next` starts right after it) and
   then into the preceding one (size non-zero, ending right at the header), each merge reclaiming
   0x10 header bytes and one block. Returns 1 on success, 0 without a state, for NULL or a bad tag
   (the `MemListRemove` result is tested but it only fails for a NULL block; list membership is
   not checked). */
int MemLocalHeapFree(MemLocalHeap *self, void *ptr)
{
    MemBlock *blk;
    MemBlock *prev;

    if (self->state == NULL || ptr == NULL) {
        return 0;
    }
    blk = (MemBlock *)ptr - 1;
    if (blk->data != ptr || !MemListRemove(&self->state->usedList, blk)) {
        return 0;
    }
    MemListInsert(&self->state->freeList, blk, true);
    self->state->freeSize += blk->size;
    self->state->usedSize -= blk->size;

    if (blk->next != NULL && (MemBlock *)((u8 *)blk->data + blk->size) == blk->next) {
        blk->size = blk->next->size + blk->size + sizeof(MemBlock);
        MemListRemove(&self->state->freeList, blk->next);
        self->state->freeSize += sizeof(MemBlock);
        self->state->blockCount--;
    }
    prev = blk->prev;
    if (prev != NULL && prev->size != 0 && (MemBlock *)((u8 *)prev->data + prev->size) == blk) {
        prev->size = blk->size + prev->size + sizeof(MemBlock);
        MemListRemove(&self->state->freeList, blk);
        self->state->freeSize += sizeof(MemBlock);
        self->state->blockCount--;
    }
    return 1;
}
