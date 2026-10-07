// bdc 0x08a275d0 GmoTgaBppToFormat
#include "bdc.h"

/* Maps a TGA pixel depth to a GE pixel format: 8 → 5 (T8), 16 → 1 (5551), 24/32 → 3 (8888);
   -1 otherwise. */

s32 GmoTgaBppToFormat(s32 bpp)
{
    s32 fmt;

    fmt = 5;
    if ((bpp != 8) && (fmt = 1, bpp != 0x10)) {
        fmt = -1;
        if (bpp == 0x18) {
            fmt = 3;
        } else if (bpp == 0x20) {
            fmt = 3;
        }
    }
    return fmt;
}
