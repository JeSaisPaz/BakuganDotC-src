// bdc 0x08a13584 GmoHeapAlloc
#include "bdc.h"

/* Allocates `size` bytes aligned to `align` from pool `pool` of the model library's 3-pool block
   heap (`g_gmoHeapPools`; same design as the image library's `GmoImageHeapAllocBlock` heap):
   calls the pool allocator for size + alignment slack + a 0x10-byte `GmoImageBlock` trailer,
   places the trailer after the 4-aligned data (refcount 1), links it at the head of the pool's
   block list and widens the pool's address range. Returns the data pointer, or NULL when `size` is
   0 or the allocator fails. */

void *GmoHeapAlloc(int pool, u32 align, int size)
{
  GmoImagePool *p;
  u32 poolAlign;
  u32 effAlign;
  u32 padding;
  u8 *raw;
  u8 *data;
  GmoImageBlock *block;
  GmoImageBlock *head;

  if (size == 0) {
    return NULL;
  }
  p = &g_gmoHeapPools[pool];
  poolAlign = p->align;
  effAlign = poolAlign;
  padding = 3;
  if (poolAlign <= align) {
    effAlign = align;
  }
  if (effAlign > 3) {
    padding = -size & 3;
  }
  raw = p->alloc(padding + size + (effAlign - poolAlign) + 0x10);
  if (raw == NULL) {
    return NULL;
  }
  data = (u8 *)(((uintptr_t)raw + align - 1) & -(uintptr_t)align);
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
  block->refCount = 1;
  block->prev = head;
  block->pool = (u8)pool;
  block->alloc = raw;
  block->next = NULL;
  block->offset = (u8)(data - raw);
  return raw + (u8)(data - raw);
}
