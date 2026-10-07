// bdc 0x08a0fb8c GmoImageNullAlloc
#include "bdc.h"

/* Null allocator of the image library: returns NULL. Default of the third allocator slot
   `0x08af1248` (after the main and VRAM slots set by
   `GmoImageSetAllocator`/`GmoImageSetVramAllocator`). */
void *GmoImageNullAlloc(s32 size)
{
    (void)size;
    return NULL;
}
