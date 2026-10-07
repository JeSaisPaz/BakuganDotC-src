// bdc 0x08a0fcb0 GmoImageHeapSetVramPool
#include "bdc.h"

/* Configures pool 1 (VRAM) of the image library's 3-pool block heap (`g_gmoImagePools`) with
   `alloc`, `free`, `align` and `flag` (`GmoImageHeapSetPool`), then, unless both `alloc` and
   `free` were given, overwrites pool 1's allocator settings with pool 0's
   (`GmoImageHeapInheritPool`). */

void GmoImageHeapSetVramPool(void *alloc, void *free, u16 align, u8 flag)
{
  GmoImageHeapSetPool(1, alloc, free, align, flag);
  GmoImageHeapInheritPool(1, 0);
}
