// bdc 0x089be5dc CoreLzssDecodeRaw
#include "bdc.h"

/* Okumura LZSS decoder used by `CoreLzssDecompress`: decodes from `src` (past the size header)
   into `dst` until at least `size` bytes are produced, with a 4 KiB ring buffer (0x1011 bytes
   allocated from the low heap end, first 0xfee bytes zeroed, start position 0xfee). Flag bit 1 =
   literal byte; 0 = 2-byte match (12-bit position, 4-bit length + 3). A match is copied in full
   before the size test, so the output may overrun `size` by up to 17 bytes. */
void CoreLzssDecodeRaw(const u8 *src, u8 *dst, u32 size)
{
    u8 *window;
    bool fromLow;
    u32 ringPos = 0xfee;
    u32 outCount = 0;
    u32 flags = 0;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    window = MemAlloc(0x1011 * sizeof(u8), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    memset(window, 0, 0xfee /* PSP: LZSS ring prefill in bytes, not the object size */);

    for (;;) {
        if ((flags & 0x100) == 0) {
            flags = *src++ | 0xff00;
        }
        if (flags & 1) {
            u8 c = *src++;

            *dst++ = c;
            outCount++;
            if (outCount >= size) {
                break;
            }
            window[ringPos] = c;
            ringPos = (ringPos + 1) & 0xfff;
        } else {
            u8 lo = *src++;
            u8 hi = *src++;
            u32 pos = lo | (u32)(hi & 0xf0) << 4;
            s32 len = (hi & 0xf) + 2;
            s32 i;

            for (i = 0; i <= len; i++) {
                u8 c = window[(pos + i) & 0xfff];

                *dst++ = c;
                window[ringPos] = c;
                ringPos = (ringPos + 1) & 0xfff;
            }
            outCount += len + 1;
            if (outCount >= size) {
                break;
            }
        }
        flags >>= 1;
    }

    if (window != NULL) {
        MemLock();
        MemFree(window, NULL, 0);
        MemUnlock();
    }
}
