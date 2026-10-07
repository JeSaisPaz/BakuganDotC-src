// bdc 0x08a01a30 MemLocalHeapDebugWalk
#include "bdc.h"

/* Stripped debug dump of a `MemLocalHeap`: walks the used and free block lists of the heap
   header without doing anything (the print calls were compiled out). */
void MemLocalHeapDebugWalk(MemLocalHeap *self)
{
    MemMng *state = self->state;
    MemBlock *blk;

    if (state == NULL) {
        return;
    }
    if (state->usedList.head != NULL) {
        for (blk = state->usedList.head->next; blk != NULL; blk = blk->next) {
        }
    }
    if (state->freeList.head != NULL) {
        for (blk = state->freeList.head->next; blk != NULL; blk = blk->next) {
        }
    }
}
