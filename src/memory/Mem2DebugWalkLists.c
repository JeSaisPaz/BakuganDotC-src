// bdc 0x089d7354 Mem2DebugWalkLists
#include "bdc.h"

/* Stripped debug dump of `MemMng2` called by `Mem2Free` when a block is not found: walks the
   used and free block lists of the state block without doing anything (the print calls were
   compiled out). */
void Mem2DebugWalkLists(MemMng2 *self)
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
