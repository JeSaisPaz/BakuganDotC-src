// bdc 0x08a0fc1c GmoImageHeapInheritPool
#include "bdc.h"

/* Copies the allocator settings of pool `src` into pool `dst` of the image library's 3-pool block
   heap (pool records of 0x24 bytes at `0x08af1258`: allocator, free function, alignment, flags,
   block list, lookup cache, address range) unless `dst` has a custom allocator. */

void GmoImageHeapInheritPool(int dst, int src)

{
  void *(*allocFn)(int);
  void (*freeFn)(void *);
  u16 align;

  if (g_gmoImagePools[dst].custom == '\0') {
    allocFn = g_gmoImagePools[src].alloc;
    freeFn = g_gmoImagePools[src].free;
    align = g_gmoImagePools[src].align;
    g_gmoImagePools[dst].flag = g_gmoImagePools[src].flag;
    g_gmoImagePools[dst].alloc = allocFn;
    g_gmoImagePools[dst].free = freeFn;
    g_gmoImagePools[dst].align = align;
  }
  return;
}
