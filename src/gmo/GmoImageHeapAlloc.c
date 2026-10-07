// bdc 0x08a0fe24 GmoImageHeapAlloc
#include "bdc.h"

/* Allocates `size` bytes from pool `pool` of the image library's 3-pool block heap (pool records of
   0x24 bytes at `0x08af1258`: allocator, free function, alignment, flags, block list, lookup cache,
   address range) (`GmoImageHeapAllocBlock`) and returns the data pointer (block start + offset
   byte), or 0. */

void *GmoImageHeapAlloc(int pool, u32 align, int size)

{
  GmoImageBlock *block;

  if (size != 0) {
    block = (GmoImageBlock *)GmoImageHeapAllocBlock(pool, align, size);
    if (block != (GmoImageBlock *)0x0) {
      return (u8 *)block->alloc + block->offset;
    }
  }
  return (void *)0x0;
}
