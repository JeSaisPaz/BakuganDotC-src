// bdc 0x089f9034 GfxFabClipDtor
#include "bdc.h"

/* Destructor of a `.fab` clip (vtable `g_gfxFabClipVtable` slot 1): releases its placed objects (`objHead`,
   `CoreObjectListDeleteAll`) when bound, runs the base dtor and frees the clip when `flags & 1`. */

void GfxFabClipDtor(void *clip_, u32 flags)
{
    GfxFabClip *clip = clip_;
    if (clip != NULL) {
        clip->vtable = &g_gfxFabClipVtable;
        if (clip->fab != NULL) {
            CoreObjectListDeleteAll((CoreObjectList *)&clip->objHead);
        }
        CoreObjectDtor((CoreObject *)clip, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(clip, NULL, 0);
            MemUnlock();
        }
    }
}
