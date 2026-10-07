// bdc 0x089ce890 GfxDisplayCtor
#include "bdc.h"

/* Constructor of the `GfxDisplay` object: clears the VRAM allocator pointer (`+0x48`), zeroes the
   0x44-byte bit table at `+0x4c`, presets bits 0x110..0x14f of it and clears `flag90` (`+0x90`).
   Returns `display`. The remaining fields are filled in by `GfxDisplaySetup` and
   `GfxDisplayInitVram`. */

GfxDisplay *GfxDisplayCtor(GfxDisplay *display)
{
    s32 i;

    display->vramAlloc = NULL;
    memset(display->bitmap, 0, 0x44);
    for (i = 0x110; i < 0x150; i++) {
        display->bitmap[i >> 5] |= 1u << (i & 0x1f);
    }
    display->flag90 = 0;
    return display;
}
