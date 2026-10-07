// bdc 0x089050f8 BtlDemoSceneDtor
#include "bdc.h"

/* Destructor of a `.scb` demo scene (`BtlDemoScene`, 0x1c4 bytes, `BtlDemoSceneCtor`; loaded
   by `BtlDemoSceneLoad`): does nothing for NULL; otherwise `BtlDemoSceneUnload`, then
   destroys the `events` list head and the `objects` list head (flags 2: no free), and frees the
   scene under the heap lock when bit 0 of `flags` is set. */
void BtlDemoSceneDtor(void *scene, u32 flags)
{
    BtlDemoScene *self = (BtlDemoScene *)scene;

    if (self == NULL) {
        return;
    }
    BtlDemoSceneUnload(self);
    BtlDemoSceneEventListDtor(&self->events, 2);
    BtlDemoSceneObjListDtor(&self->objects, 2);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
