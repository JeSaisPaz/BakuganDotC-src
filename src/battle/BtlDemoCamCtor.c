// bdc 0x088fdbf4 BtlDemoCamCtor
#include "bdc.h"

/* Constructor of the battle demo camera (a `GfxCamera` embedded in the demo task, vtable
   `g_btlDemoCamVtbl`): runs `GfxCameraCtor` and `GfxCameraInit`, sets eye (0, 60, 90, 0),
   target (0, 0, 20, 0), near 2 / far 35000, computes the matrices (`GfxCameraUpdate` with all
   flags), then resets the demo state: the listener point and the eye/look/lens/second-track key
   vectors are zeroed (VFPU bank C720 = 0), state, frames and the auxiliary key are zeroed,
   lens scalars a = 1 / b = 0, shot id 0, fov 45, look-height blend and roll scale 1, fixed offset off.
   Returns `self`. */

void *BtlDemoCamCtor(BtlDemoCam *self)
{
    GfxCameraCtor(&self->base.base);
    self->base.base.vtable = g_btlDemoCamVtbl;
    GfxCameraInit(&self->base);
    self->base.eye[0] = 0.0f;
    self->base.eye[1] = 60.0f;
    self->base.eye[2] = 90.0f;
    self->base.eye[3] = 0.0f;
    self->base.target[0] = 0.0f;
    self->base.target[1] = 0.0f;
    self->base.target[2] = 20.0f;
    self->base.target[3] = 0.0f;
    self->base.nearZ = 2.0f;
    self->base.farZ = 35000.0f;
    GfxCameraUpdate(&self->base, 0xffffffff);
    self->listener[0] = 0.0f;
    self->listener[1] = 0.0f;
    self->listener[2] = 0.0f;
    self->listener[3] = 0.0f;
    self->state = 0;
    self->lensKey[0][0] = 0.0f;
    self->lensKey[0][1] = 0.0f;
    self->lensKey[0][2] = 0.0f;
    self->lensKey[0][3] = 0.0f;
    self->lensKey[1][0] = 0.0f;
    self->lensKey[1][1] = 0.0f;
    self->lensKey[1][2] = 0.0f;
    self->lensKey[1][3] = 0.0f;
    self->eyeKey[0][0] = 0.0f;
    self->eyeKey[0][1] = 0.0f;
    self->eyeKey[0][2] = 0.0f;
    self->eyeKey[0][3] = 0.0f;
    self->eyeKey[1][0] = 0.0f;
    self->eyeKey[1][1] = 0.0f;
    self->eyeKey[1][2] = 0.0f;
    self->eyeKey[1][3] = 0.0f;
    self->lookKey[0][0] = 0.0f;
    self->lookKey[0][1] = 0.0f;
    self->lookKey[0][2] = 0.0f;
    self->lookKey[0][3] = 0.0f;
    self->lookKey[1][0] = 0.0f;
    self->lookKey[1][1] = 0.0f;
    self->lookKey[1][2] = 0.0f;
    self->lookKey[1][3] = 0.0f;
    self->auxKey[0] = 0.0f;
    self->eyeFrame[0] = 0;
    self->eyeFrame[1] = 0;
    self->lensFrame[0] = 0;
    self->lensFrame[1] = 0;
    self->frame = 0;
    self->lensA[0] = 1.0f;
    self->lensA[1] = 1.0f;
    self->lensB[0] = 0.0f;
    self->lensB[1] = 0.0f;
    self->shotId = 0;
    self->fov = 45.0f;
    self->lookHeightBlend = 1.0f;
    self->rollScale = 1.0f;
    self->fixedOffset = 0;
    self->keyBA[0][0] = 0.0f;
    self->keyBA[0][1] = 0.0f;
    self->keyBA[0][2] = 0.0f;
    self->keyBA[0][3] = 0.0f;
    self->keyBA[1][0] = 0.0f;
    self->keyBA[1][1] = 0.0f;
    self->keyBA[1][2] = 0.0f;
    self->keyBA[1][3] = 0.0f;
    self->keyBB[0][0] = 0.0f;
    self->keyBB[0][1] = 0.0f;
    self->keyBB[0][2] = 0.0f;
    self->keyBB[0][3] = 0.0f;
    self->keyBB[1][0] = 0.0f;
    self->keyBB[1][1] = 0.0f;
    self->keyBB[1][2] = 0.0f;
    self->keyBB[1][3] = 0.0f;
    self->keyBFrame[0] = 0;
    self->keyBFrame[1] = 0;
    return self;
}
