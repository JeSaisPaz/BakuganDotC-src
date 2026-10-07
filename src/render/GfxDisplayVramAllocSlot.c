// bdc 0x089ced78 GfxDisplayVramAllocSlot
#include "bdc.h"

/* Allocates one 0x80-byte VRAM slot from the unused right edge of the frame-buffer rows for
   requests under 0x80 bytes (CLUTs): finds the first clear bit `n` in 0x40..0x21f of
   `GfxDisplay``.bitmap`, sets it and returns `edram + n*0x800 + 0x780`; returns 0 when full or
   `size >= 0x80`. */

enum { SLOT_ROW = 0x800, SLOT_OFS = 0x780 };

void *GfxDisplayVramAllocSlot(GfxDisplay *display, u32 size)
{
    u32 n;

    if (size < 0x80) {
        for (n = 0x40; n < 0x220; n++) {
            u32 bit = 1u << (n & 0x1f);

            if ((display->bitmap[n >> 5] & bit) == 0) {
                display->bitmap[n >> 5] |= bit;
                {
                    u8 *edram = (u8 *)sceGeEdramGetAddr();

                    return &edram[n * SLOT_ROW + SLOT_OFS];
                }
            }
        }
    }
    return NULL;
}
