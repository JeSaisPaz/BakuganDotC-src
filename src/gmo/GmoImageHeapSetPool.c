// bdc 0x08a0fb9c GmoImageHeapSetPool
#include "bdc.h"

/* Configures pool `pool` of the image library's 3-pool block heap (pool records of 0x24 bytes at
   `0x08af1258`: allocator, free function, alignment, flags, block list, lookup cache, address
   range): allocator (default stub `0x08a10828`) and free function (default `0x08a10830`), alignment
   `align`, flag byte, and the 'custom' byte set when both functions were given. */

void GmoImageHeapSetPool(int pool, void *alloc, void *free, u16 align, u8 flag)

{
  bool both;

  both = alloc != (void *)0x0 && free != (void *)0x0;
  if (alloc == (void *)0x0) {
    g_gmoImagePools[pool].alloc = GmoImageHeapNullAlloc;
  }
  else {
    g_gmoImagePools[pool].alloc = alloc;
  }
  g_gmoImagePools[pool].custom = both;
  if (free != (void *)0x0) {
    g_gmoImagePools[pool].free = free;
  }
  else {
    g_gmoImagePools[pool].free = GmoImageHeapNullFree;
  }
  g_gmoImagePools[pool].align = align;
  g_gmoImagePools[pool].flag = flag;
}
