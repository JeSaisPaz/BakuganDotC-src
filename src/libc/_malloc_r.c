// bdc 0x089b8f6c _malloc_r
#include "bdc.h"

/* Newlib's reentrant `_malloc_r(reent, nbytes)` (dlmalloc 2.6.x `mALLOc`): rounds the request up to
   a 16-byte-aligned chunk size (`(nbytes + 0x13) & ~0xf`, minimum 0x10; no overflow check), takes
   `__malloc_lock`, then serves it from the arena `g_mallocBins` in this order: an exact-fit
   small bin (< 0x1f8), a fit in the request's large bin, the `last_remainder` chunk (re-split,
   exhausted, or put back into its bin), the `binblocks` bitmap scan of the higher bins (splitting
   the first big-enough chunk and keeping the rest as `last_remainder`, clearing block bits found
   empty), and finally by splitting the top chunk, calling `malloc_extend_top` once when it is
   too small. Returns the chunk's user memory, or NULL (after unlocking) if the top cannot grow. */

#define MALLOC_PREV_INUSE     1
#define MALLOC_MINSIZE        0x10
#define MALLOC_MAX_SMALLBIN   63
#define MALLOC_BINBLOCK_WIDTH 4

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
    MallocChunk *bck = p->bk;
    MallocChunk *fwd = p->fd;

    fwd->bk = bck;
    bck->fd = fwd;
}

/* dlmalloc `set_inuse_bit_at_offset(p, size)`. */
static void MallocSetInuseAt(MallocChunk *p, u32 size)
{
    MallocChunkAt(p, size)->size |= MALLOC_PREV_INUSE;
}

/* dlmalloc `link_last_remainder(p)`: makes `p` the only chunk of the `last_remainder` bin. */
static void MallocLinkLastRemainder(MallocChunk *p)
{
    MallocChunk *lastRemainder = MallocBinAt(1);

    lastRemainder->bk = p;
    lastRemainder->fd = p;
    p->bk = lastRemainder;
    p->fd = lastRemainder;
}

/* dlmalloc `bin_index(size)`. */
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

/* Splits `victim` (already unlinked or the last remainder): the first `nb` bytes are returned in
   use, the `remainderSize` bytes after them become the new `last_remainder`. */
static void *MallocSplit(_reent *reent, MallocChunk *victim, u32 nb, s32 remainderSize)
{
    MallocChunk *remainder = MallocChunkAt(victim, nb);

    MallocLinkLastRemainder(remainder);
    remainder->size = remainderSize | MALLOC_PREV_INUSE;
    MallocChunkAt(remainder, remainderSize)->prevSize = remainderSize;
    __malloc_unlock(reent);
    return &victim->fd;
}

void *_malloc_r(_reent *reent, u32 nbytes)
{
    MallocChunk *victim;
    MallocChunk *bin;
    MallocChunk *lastRemainder;
    MallocChunk *bck;
    MallocChunk *fwd;
    MallocChunk *top;
    u32 nb;
    u32 victimSize;
    s32 remainderSize;
    s32 idx;
    s32 binIdx;
    s32 startIdx;
    u32 block;

    nb = MALLOC_MINSIZE;
    if ((s32)(nbytes + 0x13) >= MALLOC_MINSIZE + 0xf) {
        nb = (nbytes + 0x13) & ~(u32)0xf;
    }

    __malloc_lock(reent);

    if (nb < 0x1f8) {
        /* Small request: exact-fit bin, no size check needed. */
        idx = nb >> 3;
        bin = MallocBinAt(idx);
        victim = bin->bk;
        if (victim != bin) {
            victimSize = victim->size & ~(u32)3;
            MallocUnlink(victim);
            MallocSetInuseAt(victim, victimSize);
            __malloc_unlock(reent);
            return &victim->fd;
        }
        idx += 2; /* this bin and the next one (remainder < MINSIZE) are scanned */
    } else {
        idx = MallocBinIndex(nb);
        bin = MallocBinAt(idx);
        for (victim = bin->bk; victim != bin; victim = victim->bk) {
            victimSize = victim->size & ~(u32)3;
            remainderSize = victimSize - nb;
            if (remainderSize >= MALLOC_MINSIZE) {
                --idx; /* too big: rescan this bin below, after the last remainder */
                break;
            }
            if (remainderSize >= 0) {
                /* Exact fit. */
                MallocUnlink(victim);
                MallocSetInuseAt(victim, victimSize);
                __malloc_unlock(reent);
                return &victim->fd;
            }
        }
        ++idx;
    }

    /* Try the last split-off remainder. */
    lastRemainder = MallocBinAt(1);
    victim = lastRemainder->fd;
    if (victim != lastRemainder) {
        victimSize = victim->size & ~(u32)3;
        remainderSize = victimSize - nb;
        if (remainderSize >= MALLOC_MINSIZE) {
            /* Re-split. */
            victim->size = nb | MALLOC_PREV_INUSE;
            return MallocSplit(reent, victim, nb, remainderSize);
        }

        /* clear_last_remainder */
        lastRemainder->bk = lastRemainder;
        lastRemainder->fd = lastRemainder;

        if (remainderSize >= 0) {
            /* Exhaust it. */
            MallocSetInuseAt(victim, victimSize);
            __malloc_unlock(reent);
            return &victim->fd;
        }

        /* Too small: place it in its bin (dlmalloc `frontlink`). */
        if (victimSize < 0x200) {
            binIdx = victimSize >> 3;
            g_mallocBins.binblocks |= 1 << (binIdx / MALLOC_BINBLOCK_WIDTH);
            bck = MallocBinAt(binIdx);
            fwd = bck->fd;
        } else {
            binIdx = MallocBinIndex(victimSize);
            bck = MallocBinAt(binIdx);
            fwd = bck->fd;
            if (fwd == bck) {
                g_mallocBins.binblocks |= 1 << (binIdx / MALLOC_BINBLOCK_WIDTH);
            } else {
                while (fwd != bck && victimSize < (fwd->size & ~(u32)3)) {
                    fwd = fwd->fd;
                }
                bck = fwd->bk;
            }
        }
        victim->bk = bck;
        victim->fd = fwd;
        bck->fd = victim;
        fwd->bk = victim;
    }

    /* Scan the possibly non-empty bin blocks from `idx` upwards for the first big-enough chunk. */
    block = 1 << (idx / MALLOC_BINBLOCK_WIDTH);
    if (block <= g_mallocBins.binblocks) {
        if ((block & g_mallocBins.binblocks) == 0) {
            /* Go to the first marked block, on a block boundary. */
            idx = (idx & ~(MALLOC_BINBLOCK_WIDTH - 1)) + MALLOC_BINBLOCK_WIDTH;
            block <<= 1;
            while ((block & g_mallocBins.binblocks) == 0) {
                idx += MALLOC_BINBLOCK_WIDTH;
                block <<= 1;
            }
        }

        for (;;) {
            startIdx = idx;
            do {
                bin = MallocBinAt(idx);
                for (victim = bin->bk; victim != bin; victim = victim->bk) {
                    victimSize = victim->size & ~(u32)3;
                    remainderSize = victimSize - nb;
                    if (remainderSize >= MALLOC_MINSIZE) {
                        /* Split. */
                        victim->size = nb | MALLOC_PREV_INUSE;
                        MallocUnlink(victim);
                        return MallocSplit(reent, victim, nb, remainderSize);
                    }
                    if (remainderSize >= 0) {
                        /* Take it whole. */
                        MallocSetInuseAt(victim, victimSize);
                        MallocUnlink(victim);
                        __malloc_unlock(reent);
                        return &victim->fd;
                    }
                }
                /* Odd small bins are never used with 16-byte alignment: skip them. */
                if (idx < MALLOC_MAX_SMALLBIN) {
                    ++idx;
                }
            } while ((++idx & (MALLOC_BINBLOCK_WIDTH - 1)) != 0);

            /* Clear the block bit, backtracking over a partially scanned block. */
            for (;;) {
                if ((startIdx & (MALLOC_BINBLOCK_WIDTH - 1)) == 0) {
                    g_mallocBins.binblocks &= ~block;
                    break;
                }
                --startIdx;
                if (g_mallocBins.bins[startIdx].fd != MallocBinAt(startIdx)) {
                    break;
                }
            }

            /* Go to the next possibly non-empty block. */
            block <<= 1;
            if (block > g_mallocBins.binblocks || block == 0) {
                break;
            }
            while ((block & g_mallocBins.binblocks) == 0) {
                idx += MALLOC_BINBLOCK_WIDTH;
                block <<= 1;
            }
        }
    }

    /* Use the top chunk, which must keep at least MINSIZE bytes. */
    top = g_mallocBins.bins[0].fd;
    victimSize = top->size & ~(u32)3;
    remainderSize = victimSize - nb;
    if (victimSize < nb || remainderSize < MALLOC_MINSIZE) {
        malloc_extend_top(reent, nb);
        top = g_mallocBins.bins[0].fd;
        victimSize = top->size & ~(u32)3;
        remainderSize = victimSize - nb;
        if (victimSize < nb || remainderSize < MALLOC_MINSIZE) {
            __malloc_unlock(reent);
            return NULL;
        }
    }

    victim = top;
    victim->size = nb | MALLOC_PREV_INUSE;
    top = MallocChunkAt(victim, nb);
    g_mallocBins.bins[0].fd = top;
    top->size = remainderSize | MALLOC_PREV_INUSE;
    __malloc_unlock(reent);
    return &victim->fd;
}
