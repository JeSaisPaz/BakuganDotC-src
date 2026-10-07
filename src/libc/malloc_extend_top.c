// bdc 0x089b8c84 malloc_extend_top
#include "bdc.h"

/* Grows the malloc arena so the top chunk can satisfy a request of `nb` (already
   chunk-size-adjusted) bytes: asks `_sbrk_r` for `nb + ``g_mallocTopPad`` + 0x10` bytes
   (rounded up to the 0x1000 page size once the first sbrk has happened, tracked by
   `g_mallocSbrkBase`), then either extends the old top chunk in place when the new memory is
   contiguous or starts a fresh top (aligning the new block to 16 bytes, fixing the page boundary
   with a second sbrk, and releasing the old top remainder with `_free_r`, leaving two fencepost
   chunks behind). Updates `g_mallocSbrkedMem`, `g_mallocMaxSbrkedMem` and
   `g_mallocMaxTotalMem`; on any `_sbrk_r` failure it returns without changing the top. */

#define MALLOC_PREV_INUSE 1
#define MALLOC_MINSIZE    0x10

/* dlmalloc `mem2chunk`: the chunk whose user data (`fd` onwards) is `mem`. */
static MallocChunk *MallocMemToChunk(void *mem)
{
    return (MallocChunk *)((u8 *)mem - __builtin_offsetof(MallocChunk, fd));
}

/* dlmalloc `chunk_at_offset`. */
static MallocChunk *MallocChunkAt(MallocChunk *p, s32 offset)
{
    return (MallocChunk *)((u8 *)p + offset);
}

/* dlmalloc `bin_at(i)`: the fake chunk whose `fd`/`bk` are `bins[i]`. */
static MallocChunk *MallocBinAt(s32 i)
{
    return MallocMemToChunk(&g_mallocBins.bins[i]);
}

#define MALLOC_PAGE_SIZE   0x1000
#define MALLOC_SBRK_FAIL   ((void *)(intptr_t)-1)

void malloc_extend_top(_reent *reent, u32 nb)
{
    MallocChunk *oldTop = g_mallocBins.bins[0].fd;
    u32 oldTopSize = oldTop->size & ~(u32)3;
    u8 *oldEnd = (u8 *)MallocChunkAt(oldTop, oldTopSize);
    long oldSbrkBase = g_mallocSbrkBase;
    u32 sbrkSize;
    u8 *brk;
    u8 *newBrk;
    u32 frontMisalign;
    u32 frontCorrection;
    u32 pageMisalign;
    s32 correction;
    MallocChunk *top;

    sbrkSize = nb + g_mallocTopPad + MALLOC_MINSIZE;
    if (g_mallocSbrkBase != (intptr_t)MALLOC_SBRK_FAIL) {
        sbrkSize = (sbrkSize + MALLOC_PAGE_SIZE - 1) & ~(u32)(MALLOC_PAGE_SIZE - 1);
    }

    brk = _sbrk_r(reent, sbrkSize);
    if ((void *)brk == MALLOC_SBRK_FAIL) {
        return;
    }
    if (brk < oldEnd && oldTop != MallocBinAt(0)) {
        return;
    }

    g_mallocSbrkedMem += sbrkSize;
    if (brk == oldEnd) {
        /* Contiguous: just grow the top chunk. */
        g_mallocBins.bins[0].fd->size = (oldTopSize + sbrkSize) | MALLOC_PREV_INUSE;
    } else {
        if (g_mallocSbrkBase == (intptr_t)MALLOC_SBRK_FAIL) {
            g_mallocSbrkBase = (intptr_t)brk;
        } else {
            g_mallocSbrkedMem += brk - oldEnd;
        }

        /* Align the new top's user data to 16 bytes and end the break on a page boundary. */
        frontMisalign = (uintptr_t)&((MallocChunk *)brk)->fd & 0xf;
        frontCorrection = 0;
        if (frontMisalign != 0) {
            frontCorrection = 0x10 - frontMisalign;
            brk += frontCorrection;
        }
        pageMisalign = (uintptr_t)(brk + sbrkSize) & (MALLOC_PAGE_SIZE - 1);
        correction = frontCorrection + (MALLOC_PAGE_SIZE - pageMisalign);

        newBrk = _sbrk_r(reent, correction);
        if ((void *)newBrk == MALLOC_SBRK_FAIL) {
            g_mallocSbrkedMem -= (brk - frontCorrection) - oldEnd;
            _sbrk_r(reent, -(s32)sbrkSize);
            g_mallocSbrkBase = oldSbrkBase;
            return;
        }

        g_mallocSbrkedMem += correction;
        top = (MallocChunk *)brk;
        g_mallocBins.bins[0].fd = top;
        top->size = ((newBrk - brk) + correction) | MALLOC_PREV_INUSE;

        if (oldTop != MallocBinAt(0)) {
            if (oldTopSize < MALLOC_MINSIZE) {
                g_mallocBins.bins[0].fd->size = MALLOC_PREV_INUSE;
                return;
            }
            /* Shrink the old top and leave two fencepost chunks after it. */
            oldTopSize = (oldTopSize - 0xc) & ~(u32)0xf;
            oldTop->size = (oldTop->size & MALLOC_PREV_INUSE) | oldTopSize;
            MallocChunkAt(oldTop, oldTopSize)->size = 4 | MALLOC_PREV_INUSE;
            MallocChunkAt(oldTop, oldTopSize + 4)->size = 4 | MALLOC_PREV_INUSE;
            if (oldTopSize >= MALLOC_MINSIZE) {
                _free_r(reent, &oldTop->fd);
            }
        }
    }

    if (g_mallocSbrkedMem > g_mallocMaxSbrkedMem) {
        g_mallocMaxSbrkedMem = g_mallocSbrkedMem;
    }
    if (g_mallocSbrkedMem > g_mallocMaxTotalMem) {
        g_mallocMaxTotalMem = g_mallocSbrkedMem;
    }
}
