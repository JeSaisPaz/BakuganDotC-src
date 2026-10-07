// bdc 0x089d7974 MemAllocAligned
#include "bdc.h"

/* Allocates `size` bytes (rounded up to 0x40) aligned to 64 bytes from the main heap, from the low
   end when `fromLow` (the previous placement policy is restored afterwards): over-allocates by
   0x40 and stores the padding minus one in the byte before the returned pointer, which
   `MemFreeAligned` uses to recover the block. The result is not checked for NULL. */
void *MemAllocAligned(u32 size, bool fromLow)
{
    bool prevFromLow;
    u8 *block;
    u32 pad;

    if ((size & 0x3f) != 0) {
        size += 0x40 - (size & 0x3f);
    }
    MemLock();
    prevFromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(fromLow);
    block = MemAlloc(size + 0x40 /* PSP: 64-byte alignment slack */, NULL, 0);
    MemSetAllocFromLow(prevFromLow);
    MemUnlock();

    pad = 0x40 - ((uintptr_t)block & 0x3f);
    block[pad - 1] = (u8)(pad - 1);
    return block + pad;
}
