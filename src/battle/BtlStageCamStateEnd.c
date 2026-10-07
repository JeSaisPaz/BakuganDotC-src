// bdc 0x08904664 BtlStageCamStateEnd
#include "bdc.h"

/* State 2 of the stage camera demo task (`BtlStageCam`, task id 0x6b, `BtlStageCamCtor`).
   Step 0: when the camera task exists, rebinds the battle camera (`BtlMainRebindListener` on
   `BtlGetCameraTask`); restores the saved active camera `savedCam` into `g_gfxActiveCamera`
   when non-NULL; advances `step` to 1 and removes the task (`CoreTaskRemove`, destroy).
   Step 1 only removes the task; other steps do nothing. */
void BtlStageCamStateEnd(CoreTask *task)
{
    BtlStageCam *self = (BtlStageCam *)task;

    if (self->step < 0 || self->step > 1) {
        return;
    }
    if (self->step == 0) {
        if (BtlCameraTaskExists()) {
            BtlMainRebindListener(BtlGetCameraTask());
        }
        if (self->savedCam != NULL) {
            g_gfxActiveCamera = self->savedCam;
        }
        self->step++;
    }
    CoreTaskRemove(task, true);
}
