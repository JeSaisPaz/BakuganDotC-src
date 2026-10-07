// bdc 0x08a10828 GmoImageHeapNullAlloc
#include "bdc.h"

/* Null pool allocator of the image library's block heap: returns NULL. Static default of pool 2
   and the fallback GmoImageHeapSetPool installs when no allocator is given. */
void *GmoImageHeapNullAlloc(s32 size)
{
    (void)size;
    return NULL;
}
