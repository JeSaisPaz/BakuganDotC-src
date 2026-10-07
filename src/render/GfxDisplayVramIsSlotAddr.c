// bdc 0x089cee7c GfxDisplayVramIsSlotAddr
#include "bdc.h"

/* Returns 1 when `ptr` looks like a row-padding VRAM slot: inside the first EDRAM_FB_SIZE bytes of EDRAM
   (the frame/display buffers) and at offset 0x780 within a 0x800-byte row; else 0. */

enum { EDRAM_FB_SIZE = 0x110000, SLOT_ROW = 0x800, SLOT_OFS = 0x780 };

int GfxDisplayVramIsSlotAddr(GfxDisplay *display, void *ptr)
{
    u8 *p = (u8 *)ptr;
    u8 *edram = (u8 *)sceGeEdramGetAddr();

    if (edram < p) {
        edram = (u8 *)sceGeEdramGetAddr();
        if ((u32)(p - edram) < EDRAM_FB_SIZE) {
            edram = (u8 *)sceGeEdramGetAddr();
            u32 rel = (u32)(p - edram);

            rel -= SLOT_OFS;
            if ((rel & (SLOT_ROW - 1)) == 0) {
                return 1;
            }
        }
    }
    return 0;
}
