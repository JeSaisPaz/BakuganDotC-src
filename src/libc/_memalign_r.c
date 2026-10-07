// bdc 0x08a0f8e0 _memalign_r
#include "bdc.h"

/* Newlib's dlmalloc 2.6.x `_memalign_r(reent, alignment, size)` (`mEMALIGn`): alignments up to 16
   are plain `_malloc_r`. Otherwise it allocates `size` (rounded to a chunk size) + `alignment` +
   16 bytes, and under `__malloc_lock` carves an aligned `MallocChunk` out of it: a misaligned
   leading part (at least 16 bytes) is split off and released with `_free_r`, and so is a
   trailing remainder of at least 16 bytes. Returns the aligned memory, or NULL when the allocation
   fails. */

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

void *_memalign_r(_reent *reent, size_t alignment, size_t size)
{
    u32 nb;
    void *mem;
    MallocChunk *p;
    MallocChunk *newp;
    MallocChunk *remainder;
    u32 chunkSize;
    s32 leadSize;
    s32 remainderSize;

    if (alignment <= 16) {
        return _malloc_r(reent, size);
    }
    if (alignment < MALLOC_MINSIZE) {
        alignment = MALLOC_MINSIZE;
    }
    nb = MALLOC_MINSIZE;
    if ((s32)(size + 0x13) >= 0x1f) {
        nb = (size + 0x13) & ~(u32)0xf;
    }
    mem = _malloc_r(reent, alignment + nb + MALLOC_MINSIZE);
    if (mem == NULL) {
        return NULL;
    }
    __malloc_lock(reent);

    p = MallocMemToChunk(mem);
    chunkSize = p->size & ~(u32)3;
    if ((uintptr_t)mem % alignment != 0) {
        newp = MallocMemToChunk(
            (void *)(((uintptr_t)mem + alignment - 1) & -(uintptr_t)alignment));
        if ((u8 *)newp - (u8 *)p < MALLOC_MINSIZE) {
            newp = MallocChunkAt(newp, alignment);
        }
        leadSize = (u8 *)newp - (u8 *)p;
        chunkSize -= leadSize;
        newp->size = chunkSize | MALLOC_PREV_INUSE;
        MallocChunkAt(newp, chunkSize)->size |= MALLOC_PREV_INUSE;
        p->size = (p->size & MALLOC_PREV_INUSE) | leadSize;
        _free_r(reent, &p->fd);
        p = newp;
        chunkSize = p->size & ~(u32)3;
    }

    remainderSize = chunkSize - nb;
    if (remainderSize >= MALLOC_MINSIZE) {
        remainder = MallocChunkAt(p, nb);
        remainder->size = remainderSize | MALLOC_PREV_INUSE;
        p->size = (p->size & MALLOC_PREV_INUSE) | nb;
        _free_r(reent, &remainder->fd);
    }
    __malloc_unlock(reent);
    return &p->fd;
}
