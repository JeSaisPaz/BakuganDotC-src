// bdc 0x089d7a94 Mem2BlockInit
#include "bdc.h"

/* Fills a `MemBlock` record of a `Mem2Init` allocator: `prev = next = NULL`, `data` and `size`
   as given. Unlike `MemBlockInit` the data pointer is explicit, because Mem2 manages an address
   range (for example VRAM) whose headers are kept in a separate record array. `file`/`line` are
   debug tags and are ignored. */
void Mem2BlockInit(MemBlock *blk, void *data, u32 size, const char *file, s32 line)
{
    (void)file;
    (void)line;
    blk->prev = NULL;
    blk->next = NULL;
    blk->data = data;
    blk->size = size;
}
