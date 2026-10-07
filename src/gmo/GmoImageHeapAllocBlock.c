// bdc 0x08a0fcec GmoImageHeapAllocBlock
#include "bdc.h"

/* Allocates a block of `size` bytes aligned to `align` from pool `pool` of the image library's
   block heap (`g_gmoImagePools`): calls the pool allocator for size + alignment slack + a
   0x10-byte `GmoImageBlock` trailer, places the trailer after the 4-aligned data, links it at
   the head of the pool's block list and widens the pool's address range. Returns the trailer or
   NULL. */
int *GmoImageHeapAllocBlock(int pool, u32 align, int size)
{
    GmoImagePool *p = &g_gmoImagePools[pool];
    u32 poolAlign = p->align;
    u32 effAlign = poolAlign;
    u32 padding = 3;
    u8 *raw;
    u8 *data;
    GmoImageBlock *block;
    GmoImageBlock *head;

    if (poolAlign < align) {
        effAlign = align;
    }
    if (effAlign > 3) {
        padding = -size & 3;
    }
    raw = p->alloc(padding + (effAlign - poolAlign) + size + 0x10);
    if (raw == NULL) {
        return NULL;
    }
    data = (u8 *)(((uintptr_t)raw + (align - 1)) & -(uintptr_t)align);
    block = (GmoImageBlock *)(((uintptr_t)(data + size) + 3) & ~(uintptr_t)3);

    head = p->blocks;
    p->blocks = block;
    if (head != NULL) {
        head->next = block;
    }
    if (p->rangeLow == NULL || data < p->rangeLow) {
        p->rangeLow = data;
    }
    if (p->rangeHigh == NULL || p->rangeHigh < (u8 *)block) {
        p->rangeHigh = (u8 *)block;
    }
    block->prev = head;
    block->pool = (u8)pool;
    block->offset = (u8)(data - raw);
    block->refCount = 1;
    block->alloc = raw;
    block->next = NULL;
    return (int *)block;
}
