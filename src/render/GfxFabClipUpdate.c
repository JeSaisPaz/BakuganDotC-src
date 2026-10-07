// bdc 0x089f90d4 GfxFabClipUpdate
#include "bdc.h"

/* Advances a `.fab` clip by one frame: past the last frame it restarts (releases the placed
   objects, frame 1) unless it holds at the end (`holdAtEnd` set and the fab not looping); creates a
   0xf0-byte placed object (`GfxFabObjectCtor` + `GfxFabObjectInit`) for every placement record
   whose start frame equals the current frame, increments the frame, then applies the keyframes of
   every placed object (`GfxFabObjectApplyKeys`). Does nothing while the clip has no fab. */

void GfxFabClipUpdate(GfxFabClip *clip)
{
    GfxFabPlacement *rec;
    GfxFabObject *obj;
    GfxFabObject *next;
    void *mem;
    bool fromLow;
    u32 frame;
    int count;
    int i;

    if (clip->fab == NULL) {
        return;
    }
    frame = clip->frame;
    if (clip->def->lastFrame < frame &&
        (clip->holdAtEnd == 0 || ((GfxFab *)clip->fab)->loop != 0)) {
        CoreObjectListDeleteAll((CoreObjectList *)&clip->objHead);
        clip->frame = 1;
        frame = 1;
    }
    count = clip->def->placementCount;
    rec = (GfxFabPlacement *)(clip->def + 1);
    for (i = 0; i < count; i++) {
        if (frame == rec->startFrame) {
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            mem = MemAlloc(sizeof(GfxFabObject), NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            obj = NULL;
            if (mem != NULL) {
                GfxFabObjectCtor((GfxFabObject *)mem, &clip->objHead);
                obj = (GfxFabObject *)mem;
            }
            GfxFabObjectInit(obj, (GfxFab *)clip->fab, clip, &rec->startFrame, rec->keys);
            frame = clip->frame;
        }
        rec = (GfxFabPlacement *)((u8 *)rec + rec->size);
    }
    clip->frame = frame + 1;
    for (obj = (GfxFabObject *)clip->objHead; obj != NULL; obj = next) {
        next = (GfxFabObject *)obj->base.next;
        GfxFabObjectApplyKeys(obj);
    }
}
