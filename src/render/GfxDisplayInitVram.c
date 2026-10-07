// bdc 0x089cf1fc GfxDisplayInitVram
#include "bdc.h"

/* Sets up the VRAM layout of the display: `vramUsed = 0x154000` (draw + display + depth buffers),
   `unk30 = 0x100`, `vramFreeStart = edram + 0x154000`, `vramPoolStart` and `vramPoolStart2`
   `= edram + 0x174000`, `vramEnd = edram + 0x200000`, then replaces the VRAM allocator
   `vramAlloc`: destroys the old one (`Mem2Destroy`, flags 3) and builds a new `Mem2Init`
   allocator (allocated from the low end of the game heap) over [`vramPoolStart`, `vramEnd`) with
   0x1000 block records and 16-byte alignment, and clears its `flag19`. The `flag19` store is
   unconditional, so a failed `MemAlloc` writes through NULL. */

void GfxDisplayInitVram(GfxDisplay *display)
{
    bool fromLow;
    u8 *edram;
    MemMng2 *mem2;
    MemMng2 *alloc;

    display->vramUsed = 0x154000;
    display->unk30 = 0x100;
    edram = sceGeEdramGetAddr();
    display->vramFreeStart = edram + 0x154000;
    display->vramPoolStart = edram + 0x174000;
    display->vramPoolStart2 = edram + 0x174000;
    edram = sceGeEdramGetAddr();
    display->vramEnd = edram + 0x200000;
    if (display->vramAlloc != NULL) {
        Mem2Destroy((MemMng2 *)display->vramAlloc, 3);
        display->vramAlloc = NULL;
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem2 = (MemMng2 *)MemAlloc(sizeof(MemMng2), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    alloc = NULL;
    if (mem2 != NULL) {
        Mem2Init(mem2, display->vramPoolStart,
                 (u32)((u8 *)display->vramEnd - (u8 *)display->vramPoolStart), 0x1000, 0x10, true);
        alloc = mem2;
    }
    display->vramAlloc = alloc;
    alloc->flag19 = 0;
}
