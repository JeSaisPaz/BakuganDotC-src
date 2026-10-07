// bdc 0x0884c264 BtlMainStartCameraCloseUp
#include "bdc.h"

/* Forwards to `BtlCameraStartCloseUp` on the follow-camera controller embedded in `BtlMain`. */
void BtlMainStartCameraCloseUp(float blend, float param, BtlMain *self, int frames)
{
    BtlCameraStartCloseUp(blend, param, &self->camera, frames);
}
