// bdc 0x08909f84 UiScreenAnimIsDone
#include "bdc.h"

/* Returns the screen's sticky animation-done flag `+0x68`; sets it once the first track of the
   animation in `slot` reaches `frame` (or its length when `frame` is -1). */

bool UiScreenAnimIsDone(void *screen, u32 frame, int slot)
{
    UiScreen *s = (UiScreen *)screen;
    GfxFab *fab;
    GfxFabClip *clip;

    if (s->unk68 != 0) {
        return true;
    }
    fab = ((GfxFab **)s->bgData)[slot];
    if (fab != NULL) {
        if (frame == 0xffffffff) {
            frame = GfxFabGetFrameCount(fab);
            fab = ((GfxFab **)s->bgData)[slot];
        }
        clip = GfxFabGetClip(fab, 0);
        if (clip != NULL && frame <= clip->frame) {
            s->unk68 = 1;
        }
    }
    return s->unk68;
}
