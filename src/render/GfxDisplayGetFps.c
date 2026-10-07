// bdc 0x089ceb74 GfxDisplayGetFps
#include "bdc.h"

/* Returns the target frames per second of the display object `display` (`g_gfxDisplay`) from its
   frame-skip interval at `+0x1c`: 0 -> 60, 1 -> 30, 2 -> 20, 3 -> 15 (every 1..4 vblanks); a
   negative or larger interval returns 0. Callers multiply it with seconds to get frame counts (e.g.
   `GfxDrawLoadMeter`: `n * fps / 60`). */

s32 GfxDisplayGetFps(void *display)
{
    s32 skip = ((GfxDisplay *)display)->frameSkip;

    if (skip < 2) {
        if (skip < 0) {
            return 0;
        }
        if (skip > 0) {
            return 30;
        }
        return 60;
    }
    if (skip < 3) {
        return 20;
    }
    if (skip < 4) {
        return 15;
    }
    return 0;
}
