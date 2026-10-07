// bdc 0x089f88e8 GfxFabSeek
#include "bdc.h"

/* Restarts the root clip (releases its placed objects, frame = 0) and replays `GfxFabUpdate`
   until `frame` is reached. */

void GfxFabSeek(GfxFab *fab, u32 frame)
{
    CoreObjectListDeleteAll((CoreObjectList *)&fab->rootClip->objHead);
    fab->rootClip->frame = 0;
    while (fab->rootClip->frame < frame) {
        GfxFabUpdate(fab);
    }
}
