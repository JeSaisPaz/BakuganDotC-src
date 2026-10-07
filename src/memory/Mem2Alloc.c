// bdc 0x089d702c Mem2Alloc
#include "bdc.h"

/* Allocates `size` bytes from a Mem2 allocator: rounds `size` up to the allocator alignment, finds
   the first block of the address-ordered free list that is large enough, splits off the remainder
   into a new record when the block is bigger, moves the block to the used list, updates the
   used/free byte counts and returns the block's `data`. Returns NULL when there is no state or no
   fitting block. `file`/`line` only tag the split-off record. */
void *Mem2Alloc(MemMng2 *self, u32 size, const char *file, s32 line)
{
    MemMng *state = self->state;
    MemBlock *blk;
    u32 rem;

    if (state == NULL) {
        return NULL;
    }
    rem = size & (self->align - 1);
    if (rem != 0) {
        size += self->align - rem;
    }
    for (blk = state->freeList.head; blk != NULL; blk = blk->next) {
        if (blk->size >= size) {
            break;
        }
    }
    if (blk == NULL) {
        return NULL;
    }
    if (size < blk->size) {
        MemBlock *tail;

        MemListRemove(&state->freeList, blk);
        tail = Mem2AllocRecord(self);
        Mem2BlockInit(tail, (u8 *)blk->data + size, blk->size - size, file, line);
        MemListInsert(&self->state->freeList, tail, true);
        blk->size = size;
        self->state->blockCount++;
    } else {
        MemListRemove(&state->freeList, blk);
    }
    MemListInsert(&self->state->usedList, blk, false);
    self->state->usedSize += blk->size;
    self->state->freeSize -= blk->size;
    return blk->data;
}
