// bdc 0x089d7198 Mem2Free
#include "bdc.h"

/* Frees a Mem2 allocation: locates its used block (`Mem2FindUsedBlock`; when missing it runs the
   empty debug walk `Mem2DebugWalkLists`), moves it back to the address-ordered free list
   (`MemListInsert`), fixes the used/free counters and merges it with the physically adjacent
   following and preceding free blocks (releasing their records with `Mem2ReleaseRecord`; a
   preceding block of size 0, i.e. the list header, is never merged). Returns true on success,
   false if `ptr` is NULL, the allocator has no state or the block is not in the used list. */
bool Mem2Free(MemMng2 *self, void *ptr)
{
    MemBlock *blk;
    MemBlock *prev;

    if (self->state == NULL || ptr == NULL) {
        return false;
    }
    blk = Mem2FindUsedBlock(self, ptr);
    if (blk == NULL) {
        Mem2DebugWalkLists(self);
        return false;
    }
    if (!MemListRemove(&self->state->usedList, blk)) {
        return false;
    }
    MemListInsert(&self->state->freeList, blk, true);
    self->state->freeSize += blk->size;
    self->state->usedSize -= blk->size;

    if (blk->next != NULL && (u8 *)blk->data + blk->size == blk->next->data) {
        MemBlock *next = blk->next;

        blk->size += next->size;
        MemListRemove(&self->state->freeList, next);
        Mem2ReleaseRecord(self, next);
        self->state->blockCount--;
    }
    prev = blk->prev;
    if (prev != NULL && prev->size != 0 && (u8 *)prev->data + prev->size == blk->data) {
        prev->size += blk->size;
        MemListRemove(&self->state->freeList, blk);
        Mem2ReleaseRecord(self, blk);
        self->state->blockCount--;
    }
    return true;
}
