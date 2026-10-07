// bdc 0x08a015b0 MemLocalHeapCtor
#include "bdc.h"

#define LOCAL_HEAP_SRC "c:/bullets/bkn2pspsys/src/pspsys/sys/Mem/COMemMng.cpp"

/* Constructor of a `MemLocalHeap` over a caller buffer (`COMemMng.cpp`, vtable
   `g_memLocalHeapVtbl`): the buffer starts with the `MemMng` header (`state`), whose first
   block sits right after it; total size `size`, used 0, free `size - 0x40`, block count 0, empty
   used/free `MemBlockList`s; one free block spanning the rest is inserted (`MemBlockInit`,
   `MemListInsert`) and the block count becomes 1. Unlike `MemInit`, `freeSize` keeps the
   block header's 0x10 bytes. Sets `flag0c = 0`, `flag0d = 1`. Returns `self`. */
void *MemLocalHeapCtor(MemLocalHeap *self, void *buffer, u32 size, u32 align)
{
    MemMng *state = buffer;
    MemBlock *blk;

    self->vtbl = g_memLocalHeapVtbl;
    self->align = align;
    self->state = state;
    state->pool = (MemBlock *)(state + 1);
    self->state->totalSize = size;
    self->state->usedSize = 0;
    self->state->freeSize = self->state->totalSize - 0x40;
    self->state->blockCount = 0;
    self->state->unk18 = 0;
    /* The list headers are block-shaped sentinels; the init sets their cursor slot, cleared below. */
    MemBlockInit((MemBlock *)&self->state->usedList, 0, LOCAL_HEAP_SRC, 0x58);
    MemBlockInit((MemBlock *)&self->state->freeList, 0, LOCAL_HEAP_SRC, 0x59);
    self->state->usedList.cursor = NULL;
    self->state->freeList.cursor = NULL;
    blk = self->state->pool;
    MemBlockInit(blk, self->state->freeSize - 0x10, LOCAL_HEAP_SRC, 0x5e);
    MemListInsert(&self->state->freeList, blk, true);
    self->state->blockCount++;
    self->flag0c = 0;
    self->flag0d = 1;
    return self;
}
