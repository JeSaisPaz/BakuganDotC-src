// bdc 0x089b95a0 _free_r
#include "bdc.h"

/* Newlib's dlmalloc 2.6.x `_free_r(reent, mem)` (`fREe`): does nothing for NULL; otherwise, under
   `__malloc_lock`, returns the `MallocChunk` of `mem` to the arena (`g_mallocBins`). A chunk
   next to `top` is merged into it (with a free predecessor too) and, when `top` reaches
   `g_mallocTrimThreshold`, `_malloc_trim_r` gives memory back with `g_mallocTopPad` slack.
   Otherwise it coalesces with a free predecessor and successor (keeping the `last_remainder` bin
   when one of them is the remainder), writes the boundary tags and links the chunk into its size
   bin (small bins: exact 8-byte classes below 0x200; large bins sorted by decreasing size), marking
   the bin block in `binblocks`. */

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

static void MallocUnlink(MallocChunk *p)
{
    MallocChunk *fwd = p->fd;
    MallocChunk *bck = p->bk;

    fwd->bk = bck;
    bck->fd = fwd;
}

static s32 MallocBinIndex(u32 size)
{
    u32 hi = size >> 9;

    if (hi == 0) {
        return size >> 3;
    }
    if (hi < 5) {
        return (size >> 6) + 56;
    }
    if (hi < 21) {
        return hi + 91;
    }
    if (hi < 85) {
        return (size >> 12) + 110;
    }
    if (hi < 341) {
        return (size >> 15) + 119;
    }
    if (hi < 1365) {
        return (size >> 18) + 124;
    }
    return 126;
}

void _free_r(_reent *reent, void *mem)
{
    MallocChunk *p;
    MallocChunk *next;
    MallocChunk *top;
    MallocChunk *lastRemainder;
    MallocChunk *bck;
    MallocChunk *fwd;
    u32 head;
    u32 size;
    u32 nextSize;
    u32 prevSize;
    s32 idx;
    int isLastRemainder;

    if (mem == NULL) {
        return;
    }
    __malloc_lock(reent);

    p = MallocMemToChunk(mem);
    head = p->size;
    size = head & ~(u32)MALLOC_PREV_INUSE;
    next = MallocChunkAt(p, size);
    nextSize = next->size & ~(u32)3;
    top = g_mallocBins.bins[0].fd;

    if (next == top) {
        /* Merge into the top chunk. */
        size += nextSize;
        if (!(head & MALLOC_PREV_INUSE)) {
            prevSize = p->prevSize;
            p = MallocChunkAt(p, -(s32)prevSize);
            size += prevSize;
            MallocUnlink(p);
        }
        p->size = size | MALLOC_PREV_INUSE;
        g_mallocBins.bins[0].fd = p;
        if (size >= g_mallocTrimThreshold) {
            _malloc_trim_r(reent, g_mallocTopPad);
        }
        __malloc_unlock(reent);
        return;
    }

    next->size = nextSize; /* clear next's PREV_INUSE */
    lastRemainder = MallocBinAt(1);
    isLastRemainder = 0;

    if (!(head & MALLOC_PREV_INUSE)) {
        prevSize = p->prevSize;
        p = MallocChunkAt(p, -(s32)prevSize);
        size += prevSize;
        if (p->fd == lastRemainder) {
            isLastRemainder = 1;
        } else {
            MallocUnlink(p);
        }
    }

    if (!(MallocChunkAt(next, nextSize)->size & MALLOC_PREV_INUSE)) {
        size += nextSize;
        if (!isLastRemainder && next->fd == lastRemainder) {
            /* Take over the last_remainder bin. */
            isLastRemainder = 1;
            lastRemainder->fd = p;
            lastRemainder->bk = p;
            p->bk = lastRemainder;
            p->fd = lastRemainder;
        } else {
            MallocUnlink(next);
        }
    }

    p->size = size | MALLOC_PREV_INUSE;
    MallocChunkAt(p, size)->prevSize = size;

    if (!isLastRemainder) {
        if (size < 0x200) {
            idx = size >> 3;
            g_mallocBins.binblocks |= 1 << (idx / 4);
            bck = MallocBinAt(idx);
            fwd = bck->fd;
        } else {
            idx = MallocBinIndex(size);
            bck = MallocBinAt(idx);
            fwd = bck->fd;
            if (fwd == bck) {
                g_mallocBins.binblocks |= 1 << (idx / 4);
            } else {
                while (fwd != bck && size < (fwd->size & ~(u32)3)) {
                    fwd = fwd->fd;
                }
                bck = fwd->bk;
            }
        }
        p->bk = bck;
        p->fd = fwd;
        bck->fd = p;
        fwd->bk = p;
    }
    __malloc_unlock(reent);
}
