// bdc 0x08a12cd4 GmoHeapSetVramPool
#include "bdc.h"

/* Configures pool 1 of the model library's 3-pool block heap (`g_gmoHeapPools`; same design as
   the image library's `GmoImageHeapAllocBlock` heap): allocator/free (defaults
   `GmoHeapNullAlloc`/`GmoHeapNullFree`), alignment and flag, and the 'custom' byte; when no
   custom pair is given, pool 1 copies pool 0's allocator, free function, alignment and flag. */
void GmoHeapSetVramPool(void *alloc, void *free, u16 align, u8 flag)
{
    u8 both = (alloc != NULL) && (free != NULL);

    if (alloc == NULL) {
        g_gmoHeapPools[1].alloc = (void *(*)(s32))GmoHeapNullAlloc;
    } else {
        g_gmoHeapPools[1].alloc = (void *(*)(s32))alloc;
    }
    if (free == NULL) {
        g_gmoHeapPools[1].free = (void (*)(void *))GmoHeapNullFree;
    } else {
        g_gmoHeapPools[1].free = (void (*)(void *))free;
    }
    g_gmoHeapPools[1].align = align;
    g_gmoHeapPools[1].flag = flag;
    g_gmoHeapPools[1].custom = both;
    if (!both) {
        g_gmoHeapPools[1].alloc = g_gmoHeapPools[0].alloc;
        g_gmoHeapPools[1].flag = g_gmoHeapPools[0].flag;
        g_gmoHeapPools[1].free = g_gmoHeapPools[0].free;
        g_gmoHeapPools[1].align = g_gmoHeapPools[0].align;
    }
}
