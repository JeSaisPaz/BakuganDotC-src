// bdc 0x08849320 BtlSlowMotionTaskDtor
#include "bdc.h"

/* Destructor of the slow-motion task: restores its vtable, clears the battle main task's
   white-flash target when that task exists, resets the motion time scale to 1.0, destroys the
   embedded camera (flags 2) and the task base, and frees the object when bit 0 of `flags` is set.
   Does nothing for NULL. */

void BtlSlowMotionTaskDtor(CoreTask *task, u32 flags)
{
    BtlSlowMotionTask *self = (BtlSlowMotionTask *)task;

    if (self == NULL) {
        return;
    }
    self->base.vtable = g_btlSlowMotionTaskVtbl;
    if (BtlCameraTaskExists() != 0) {
        ((BtlMain *)BtlGetCameraTask())->flashTarget = 0.0f;
    }
    GfxSetMotionTimeScale(1.0f);
    GfxCameraDtor(&self->camera.base, 2);
    CoreTaskDestroy(&self->base, 0);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
