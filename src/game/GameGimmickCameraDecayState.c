// bdc 0x088d851c GameGimmickCameraDecayState
#include "bdc.h"

/* State 2 of the surveillance camera gimmick (`GameGimmickCameraCtor`; member table `0x08a96b8c`:
   0 `GameGimmickCameraScanState`, 1 `GameGimmickCameraCooldownState`, 2 this): on step 0
   (`+0x180`) sets the value `+0x188 = 1`; then each frame multiplies it by 0.8 and calls virtual
   `+0x34`, until its square drops below 0.1, when it zeroes it and moves to step 2. */

void GameGimmickCameraDecayState(GameGimmickCamera *obj)
{
    int step = obj->step;
    float decay;

    if (step < 1) {
        if (step >= 0) {
            obj->decay = 1.0f;
            obj->step = 1;
            return;
        }
    } else if (step < 2) {
        decay = obj->decay;
        if (decay * decay < 0.1f) {
            obj->decay = 0.0f;
            obj->step = 2;
        } else {
            obj->decay = decay * 0.8f;
        }
        {
            const VtblEntry *e = (const VtblEntry *)obj->base.base.base.vtable + 6;
            ((void (*)(void *))e->fn)((char *)obj + e->delta);
        }
    }
}
