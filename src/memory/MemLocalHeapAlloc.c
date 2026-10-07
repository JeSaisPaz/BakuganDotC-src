// bdc 0x08a01740 MemLocalHeapAlloc
#include "bdc.h"

/* Allocates `size` bytes (rounded up to the heap alignment) from a `MemLocalHeap`: first-fit
   search of the free list; when the block can also hold a 0x10-byte header after the request, the
   remainder becomes a new free block (`MemBlockInit`, block count + 1, free bytes - 0x10).
   The block moves to the used list and the used/free counters are updated. Returns the data
   pointer, or NULL when there is no header or no fitting block. */
void *MemLocalHeapAlloc(MemLocalHeap *self, u32 size, char *file, s32 line)
{
    MemMng *state = self->state;
    MemBlock *blk;
    MemBlock *rest;
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

    if (blk->size < size + 0x10) {
        MemListRemove(&state->freeList, blk);
    } else {
        MemListRemove(&state->freeList, blk);
        rest = (MemBlock *)((u8 *)blk->data + size);
        MemBlockInit(rest, blk->size - size - 0x10, file, line);
        MemListInsert(&self->state->freeList, rest, true);
        blk->size = size;
        self->state->blockCount++;
        self->state->freeSize -= 0x10;
    }
    MemListInsert(&self->state->usedList, blk, false);
    self->state->usedSize += blk->size;
    self->state->freeSize -= blk->size;
    return blk->data;
}
