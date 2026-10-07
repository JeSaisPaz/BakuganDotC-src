// bdc 0x089b7640 _vfiprintf_write
#include "bdc.h"

/* Buffered output sink of the stream path of `_vfiprintf_r` (same routine as `_vfprintf_write` over its own buffer state). With `flush == 1` it writes whatever
   is pending in `g_vfiprintfBuf` to `fd` with `_write`, empties the buffer and returns the `_write`
   result. Otherwise it appends `len` bytes of `buf` to the 128-byte buffer and writes it out with
   `_write` each time the count passes 0x7f (a `_write` result of 0 aborts and returns 0);
   returns the number of bytes consumed. */

u32 _vfiprintf_write(s32 fd, void *buf, u32 len, s32 flush)
{
    const u8 *src = buf;
    u32 done;
    int written;

    if (flush == 1) {
        written = _write(fd, g_vfiprintfBuf, g_vfiprintfBufCount);
        g_vfiprintfBufCount = 0;
        g_vfiprintfBufPtr = g_vfiprintfBuf;
        return written;
    }
    for (done = 0; done < len; done++) {
        *g_vfiprintfBufPtr = src[done];
        g_vfiprintfBufPtr++;
        g_vfiprintfBufCount++;
        if (g_vfiprintfBufCount >= 0x80) {
            written = _write(fd, g_vfiprintfBuf, g_vfiprintfBufCount);
            g_vfiprintfBufPtr = g_vfiprintfBuf;
            g_vfiprintfBufCount = 0;
            if (written == 0) {
                return 0;
            }
        }
    }
    return done;
}
