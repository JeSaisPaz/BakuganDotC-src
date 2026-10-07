// bdc 0x089b4890 _sbrk
#include "bdc.h"

/* Newlib's `_sbrk(incr)` backend: moves the program break of the libc heap inside one fixed block
   and returns the previous break, or `(void *)-1` on failure. The block is created lazily on the
   first call: `sceKernelAllocPartitionMemory(PSP_MEMORY_PARTITION_USER, "UserSbrk",
   PSP_SMEM_LowAligned, size, 0x1000)` with `size = ``g_sceNewlibHeapKbSize`` * 1024` (0x5000 KiB
   in this image; 64 KiB if the weak size variable were absent); with a size of 0 or a failed
   allocation (UID <= 0) it returns -1. The break never moves outside `[``g_sbrkHeapBase``,
   ``g_sbrkHeapEnd``]`; a request that would leave the range returns -1 and leaves the break
   unchanged. */

/* The SDK declares `sce_newlib_heap_kb_size` weak so a program may leave it out; `_sbrk` tests its
   address (`lui/addiu 0x08aac760; beq a0,zero`) before reading it. This image defines it (0x5000),
   so the 64 KiB fallback is never taken. */
extern u32 g_sceNewlibHeapKbSize __attribute__((weak));

#define SBRK_FAIL ((void *)(intptr_t)-1)

void *_sbrk(s32 incr)
{
    u32 size;
    void *prev;
    u8 *next;

    if (g_sbrkHeapBase == NULL) {
        if (&g_sceNewlibHeapKbSize != NULL) {
            if (g_sceNewlibHeapKbSize == 0) {
                return SBRK_FAIL;
            }
            size = g_sceNewlibHeapKbSize * 1024;
        } else {
            size = 0x10000;
        }
        if (size != 0) {
            g_sbrkBlockId = sceKernelAllocPartitionMemory(2, "UserSbrk", 3, size, 0x1000);
            if (g_sbrkBlockId > 0) {
                g_sbrkHeapPtr = sceKernelGetBlockHeadAddr(g_sbrkBlockId);
                g_sbrkHeapEnd = (u8 *)g_sbrkHeapPtr + size;
                g_sbrkHeapBase = g_sbrkHeapPtr;
            }
        }
    }
    if (g_sbrkHeapBase == NULL) {
        return SBRK_FAIL;
    }
    prev = g_sbrkHeapPtr;
    next = (u8 *)prev + incr;
    if ((void *)next < g_sbrkHeapBase || g_sbrkHeapEnd < (void *)next) {
        return SBRK_FAIL;
    }
    g_sbrkHeapPtr = next;
    return prev;
}
