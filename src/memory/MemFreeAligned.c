// bdc 0x089d7a28 MemFreeAligned
#include "bdc.h"

/* Frees a pointer returned by `MemAllocAligned`: the byte just before `ptr` stores the padding
   minus one, so the original block is `ptr - 1 - ((u8 *)ptr)[-1]`, which is released with
   `MemFree` inside `MemLock`/`MemUnlock`. NULL (and a recovered NULL block) is ignored. */
void MemFreeAligned(void *ptr)
{
    u8 *bytes = ptr;
    u32 pad;
    u8 *block;

    if (ptr == NULL) {
        return;
    }
    pad = bytes[-1] + 1u;
    block = bytes - pad;
    if (block == NULL) {
        return;
    }
    MemLock();
    MemFree(block, NULL, 0);
    MemUnlock();
}
