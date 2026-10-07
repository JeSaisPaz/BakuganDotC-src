// bdc 0x089d6d34 Mem2Init
#include "bdc.h"

/* Constructor of `MemMng2` (`COMemMng2`, a second allocator over an arbitrary address range,
   source `Mem/COMemMng2.cpp`): installs `g_memMng2Vtbl`, stores the allocation alignment,
   allocates the 0x40-byte state block and a record array of `maxBlocks` 16-byte `MemBlock`s from
   the game heap (low end when `fromLow`; the heap's placement policy is restored after each
   allocation), then calls `Mem2Reset`(base, size) and sets `flag18 = 0`, `flag19 = 1`.
   Returns `self`. */
void *Mem2Init(MemMng2 *self, void *base, u32 size, s32 maxBlocks, u32 align, bool fromLow)
{
    bool prevFromLow;
    MemMng *state;
    MemBlock *records;

    self->vtbl = g_memMng2Vtbl;
    self->align = align;

    MemLock();
    prevFromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(fromLow);
    state = MemAlloc(sizeof(MemMng), NULL, 0);
    MemSetAllocFromLow(prevFromLow);
    MemUnlock();
    self->state = state;

    MemLock();
    prevFromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(fromLow);
    records = MemAlloc(maxBlocks * sizeof(MemBlock), NULL, 0);
    MemSetAllocFromLow(prevFromLow);
    MemUnlock();
    self->records = records;

    self->recordCount = maxBlocks;
    Mem2Reset(self, base, size);
    self->flag18 = 0;
    self->flag19 = 1;
    return self;
}
