// bdc 0x089d7a7c MemBlockInit
#include "bdc.h"

/* Initialises a `MemBlock` header in place: `prev = next = NULL`, `data` = the bytes right after
   the header, `size = size`. `file`/`line` are debug tags ignored in this build. */
void MemBlockInit(MemBlock *blk, u32 size, const char *file, s32 line)
{
    (void)file;
    (void)line;
    blk->prev = NULL;
    blk->next = NULL;
    blk->data = blk + 1;
    blk->size = size;
}
