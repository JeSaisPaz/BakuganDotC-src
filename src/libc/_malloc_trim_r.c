// bdc 0x089b984c _malloc_trim_r
#include "bdc.h"

/* Newlib's `_malloc_trim_r(reent, pad)` (`malloc_trim`): gives surplus memory at the end of the top
   chunk back to the system. Under the allocator lock it computes the whole pages above `pad`
   (`((top_size - pad + 0xfef) / 0x1000 - 1) * 0x1000`); if at least one page is releasable and the
   heap really ends at the top chunk (`_sbrk_r(0) == top + top_size`), it shrinks the break with
   `_sbrk_r(-extra)`, shrinks the top chunk and `g_mallocSbrkedMem`, and returns 1. Returns 0 if
   nothing could be released; if the shrink `sbrk` fails it re-reads the break and, when at least
   0x10 bytes (`MINSIZE`) remain above `top`, repairs `top`'s size and `sbrked_mem` (from
   `g_mallocSbrkBase`). */

#define MALLOC_PREV_INUSE 1
#define MALLOC_MINSIZE    0x10
#define MALLOC_PAGE_SIZE  0x1000
#define MALLOC_SBRK_FAIL  ((void *)(intptr_t)-1)

/* dlmalloc `chunk_at_offset`. */
static MallocChunk *MallocChunkAt(MallocChunk *p, s32 offset)
{
    return (MallocChunk *)((u8 *)p + offset);
}

int _malloc_trim_r(_reent *reent, u32 pad)
{
    u32 topSize;
    s32 extra;
    u8 *brk;
    s32 newTopSize;

    __malloc_lock(reent);

    topSize = g_mallocBins.bins[0].fd->size & ~(u32)3;
    extra = ((topSize - pad - MALLOC_MINSIZE + (MALLOC_PAGE_SIZE - 1)) / MALLOC_PAGE_SIZE - 1) *
            MALLOC_PAGE_SIZE;
    if (extra < MALLOC_PAGE_SIZE) {
        /* Not enough space to release a page. */
        __malloc_unlock(reent);
        return 0;
    }

    /* Only release if the break still ends at the top chunk (nobody else sbrk'd). */
    brk = _sbrk_r(reent, 0);
    if ((void *)brk != (void *)MallocChunkAt(g_mallocBins.bins[0].fd, topSize)) {
        __malloc_unlock(reent);
        return 0;
    }

    if (_sbrk_r(reent, -extra) != MALLOC_SBRK_FAIL) {
        g_mallocBins.bins[0].fd->size = (topSize - extra) | MALLOC_PREV_INUSE;
        g_mallocSbrkedMem -= extra;
        __malloc_unlock(reent);
        return 1;
    }

    /* The shrink failed: find out what we have. */
    brk = _sbrk_r(reent, 0);
    newTopSize = brk - (u8 *)g_mallocBins.bins[0].fd;
    if (newTopSize >= MALLOC_MINSIZE) {
        g_mallocSbrkedMem = brk - (u8 *)(intptr_t)g_mallocSbrkBase;
        g_mallocBins.bins[0].fd->size = newTopSize | MALLOC_PREV_INUSE;
    }
    __malloc_unlock(reent);
    return 0;
}
