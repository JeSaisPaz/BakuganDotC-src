// bdc 0x089d746c Mem2ReleaseRecord
#include "bdc.h"

/* Returns a block record to the allocator's record array by clearing it
   (`Mem2BlockInit`(blk, NULL, 0, ...)); a record with NULL `data` is free for
   `Mem2AllocRecord`. */
void Mem2ReleaseRecord(MemMng2 *self, MemBlock *blk)
{
    (void)self;
    Mem2BlockInit(blk, NULL, 0, "c:/bullets/bkn2pspsys/src/pspsys/sys/Mem/COMemMng2.cpp", 0x84);
}
