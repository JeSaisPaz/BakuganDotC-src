// bdc 0x089f8890 GfxFabAdvanceTo
#include "bdc.h"

/* Runs `GfxFabUpdate` until the root clip's frame (`rootClip->frame`) reaches `frame`. */

void GfxFabAdvanceTo(GfxFab *fab, u32 frame)
{
    u32 cur = fab->rootClip->frame;

    while (cur < frame) {
        GfxFabUpdate(fab);
        cur = fab->rootClip->frame;
    }
}
