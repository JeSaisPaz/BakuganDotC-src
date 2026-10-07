// bdc 0x089da9e0 GmoAllocAligned
#include "bdc.h"

/* Allocator callback for aligned GMO data (alignment 0x40, see `GmoSystemInit`): forwards to
   `MemAllocAligned(size, 1)`, the aligned variant of the heap allocator. */

void *GmoAllocAligned(u32 size)
{
  return MemAllocAligned(size, true);
}
