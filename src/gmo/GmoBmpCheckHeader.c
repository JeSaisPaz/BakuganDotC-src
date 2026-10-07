// bdc 0x08a27f38 GmoBmpCheckHeader
#include "bdc.h"

/* Returns true when `size` > 0x35 (file + info header) and `data` starts with `BM`. */

s32 GmoBmpCheckHeader(const u16 *data, s32 size)
{
    s32 result;

    result = 0;
    if ((data != (u16 *)0x0) && (0x35 < size)) {
        result = (*data == 0x4d42);
    }
    return result;
}
