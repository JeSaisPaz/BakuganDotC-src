// bdc 0x089d6f1c Mem2Reset
#include "bdc.h"

/* Resets a Mem2 allocator to one big free block: clears the record array, rewrites the state block
   (`base`, `size`, used 0, free `size`, block count 0), initialises the used and free list headers
   as empty blocks, takes a record (`Mem2AllocRecord`), sets it to (`base`, `size`) and inserts it
   into the free list (`MemListInsert`); block count becomes 1. */
void Mem2Reset(MemMng2 *self, void *base, u32 size)
{
    MemBlock *blk;

    self->recordHint = 0;
    memset(self->records, 0, self->recordCount << 4);
    self->state->pool = base;
    self->state->totalSize = size;
    self->state->usedSize = 0;
    self->state->freeSize = self->state->totalSize;
    self->state->blockCount = 0;
    self->state->unk18 = 0;
    Mem2BlockInit((MemBlock *)&self->state->usedList, NULL, 0, "c:/bullets/bkn2pspsys/src/pspsys/sys/Mem/COMemMng2.cpp", 0x45);
    Mem2BlockInit((MemBlock *)&self->state->freeList, NULL, 0, "c:/bullets/bkn2pspsys/src/pspsys/sys/Mem/COMemMng2.cpp", 0x46);
    blk = Mem2AllocRecord(self);
    Mem2BlockInit(blk, self->state->pool, self->state->freeSize, "c:/bullets/bkn2pspsys/src/pspsys/sys/Mem/COMemMng2.cpp", 0x4d);
    MemListInsert(&self->state->freeList, blk, true);
    self->state->blockCount++;
}
