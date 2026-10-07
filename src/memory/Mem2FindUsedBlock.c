// bdc 0x089d73b0 Mem2FindUsedBlock
#include "bdc.h"

/* Looks up the used block whose payload address equals `ptr`: starting at the used list's cursor it
   steps `prev` when `ptr` is below the current block's `data` (signed compare) and `next`
   otherwise, for at most `state->blockCount` steps. Returns the matching block, NULL when the walk
   runs off the list, and the block reached after the last step when the step budget runs out (the
   cursor itself when `blockCount` is 0). */
MemBlock *Mem2FindUsedBlock(MemMng2 *self, void *ptr)
{
    MemBlock *blk = self->state->usedList.cursor;
    u32 count = (u32)self->state->blockCount;
    u32 steps;

    for (steps = 0; steps < count; steps++) {
        if (blk == NULL) {
            return NULL;
        }
        if (blk->data == ptr) {
            return blk;
        }
        if ((intptr_t)ptr < (intptr_t)blk->data) {
            blk = blk->prev;
        } else {
            blk = blk->next;
        }
    }
    return blk;
}
