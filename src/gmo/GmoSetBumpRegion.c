// bdc 0x089da8a4 GmoSetBumpRegion
#include "bdc.h"

/* Installs the bump region used by `GmoAlloc`/`GmoFree`: `g_gmoBumpBase = align16(base)`,
   `g_gmoBumpUsedPtr = usedPtr` (the word holding the current offset) and `g_gmoBumpSize = size`. */

void GmoSetBumpRegion(void *base, u32 *usedPtr, u32 size)

{
  g_gmoBumpUsedPtr = usedPtr;
  g_gmoBumpSize = size;
  g_gmoBumpBase = (void *)(((uintptr_t)base + 0xfU) & ~(uintptr_t)0xf);
  return;
}

