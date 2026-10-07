// bdc 0x08a12e2c GmoHeapSetMainPool
#include "bdc.h"

/* Configures pool 0 of the model library's 3-pool block heap (`g_gmoHeapPools`; same design as
   the image library's `GmoImageHeapAllocBlock` heap): allocator/free (defaults
   `GmoHeapNullAlloc`/`GmoHeapNullFree`), alignment and flag, and the 'custom' byte set when both
   functions were given; pool 1 copies these settings unless it has its own custom pair. */
void GmoHeapSetMainPool(void *alloc, void *free, u16 align, u8 flag)
{
    u8 both = (alloc != NULL) && (free != NULL);

    if (alloc == NULL) {
        g_gmoHeapPools[0].alloc = (void *(*)(s32))GmoHeapNullAlloc;
    } else {
        g_gmoHeapPools[0].alloc = (void *(*)(s32))alloc;
    }
    if (free == NULL) {
        free = (void *)GmoHeapNullFree;
    }
    g_gmoHeapPools[0].free = (void (*)(void *))free;
    g_gmoHeapPools[0].custom = both;
    g_gmoHeapPools[0].align = align;
    g_gmoHeapPools[0].flag = flag;
    if (g_gmoHeapPools[1].custom == 0) {
        g_gmoHeapPools[1].alloc = g_gmoHeapPools[0].alloc;
        g_gmoHeapPools[1].free = (void (*)(void *))free;
        g_gmoHeapPools[1].align = align;
        g_gmoHeapPools[1].flag = flag;
    }
}
