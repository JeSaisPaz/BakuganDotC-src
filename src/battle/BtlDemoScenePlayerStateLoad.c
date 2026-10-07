// bdc 0x089058e0 BtlDemoScenePlayerStateLoad
#include "bdc.h"

/* State 1 of the battle demo scene player task (`BtlDemoScenePlayerCtor`): allocates a 0x1c4-byte
   `BtlDemoScene` from the low heap and constructs it (`BtlDemoSceneCtor`; `scene` stays NULL
   when the allocation fails), loads the `.scb` of `demoId` into it (`BtlDemoSceneLoad`),
   stores the active camera `g_gfxActiveCamera` in `camera` and advances `state`. */

void BtlDemoScenePlayerStateLoad(void *task)
{
    BtlDemoScenePlayer *self = (BtlDemoScenePlayer *)task;
    BtlDemoScene *scene;
    bool wasLow;

    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    scene = MemAlloc(sizeof(BtlDemoScene), NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    if (scene != NULL) {
        BtlDemoSceneCtor(scene);
    }
    self->scene = scene;
    BtlDemoSceneLoad(scene, self->demoId);
    self->camera = (BtlDemoCam *)g_gfxActiveCamera;
    self->state = self->state + 1;
}
