// bdc 0x0897e5c8 UiCollectionSphereRotateDetailModel
#include "bdc.h"

/* In the motion view of `UiCollectionSphere` (mask bit0), Up/Down tilts
   the model (`tilt`, ±0.03 within ±0.78) and Left/Right turns it (`yaw`, ±0.03, unbounded);
   any change resets the idle timer. */

void UiCollectionSphereRotateDetailModel(UiCollectionSphere *self)
{
  PadState *pad;
  float oldTilt;
  float oldYaw;
  float tilt;

  if ((self->motionButtonMask & 1) == 0) {
    return;
  }
  pad = self->base.pad;
  oldTilt = self->tilt;
  oldYaw = self->yaw;
  tilt = oldTilt;
  if (pad->buttons & 0x10) {
    tilt = oldTilt + 0.03f;
    self->tilt = tilt;
    if (!(tilt <= 0.78f)) {
      self->tilt = 0.78f;
      tilt = 0.78f;
    }
  } else if (pad->buttons & 0x40) {
    tilt = oldTilt - 0.03f;
    self->tilt = tilt;
    if (tilt < -0.78f) {
      self->tilt = -0.78f;
      tilt = -0.78f;
    }
  }
  if (pad->buttons & 0x80) {
    self->yaw = oldYaw - 0.03f;
  } else if (pad->buttons & 0x20) {
    self->yaw = oldYaw + 0.03f;
  }
  if (oldTilt != tilt || oldYaw != self->yaw) {
    self->idleTimer = 0.0f;
  }
}
