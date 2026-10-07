// bdc 0x08a12cc4 GmoHeapNullAlloc
#include "bdc.h"

/* Default allocator of the model library's 3-pool block heap (`GmoHeapSetVramPool`,
   `GmoHeapSetMainPool` install it when no allocator is given): returns NULL. */
void *GmoHeapNullAlloc(u32 size)
{
    (void)size;
    return NULL;
}
