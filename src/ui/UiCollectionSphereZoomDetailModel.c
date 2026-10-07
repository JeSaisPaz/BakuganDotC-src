// bdc 0x0897e734 UiCollectionSphereZoomDetailModel
#include "bdc.h"

/* In the motion view of `UiCollectionSphere` (mask bit1), R/L zooms the
   model (`+0x1378`, ±0.015 within 0.6..1.4) and resets the idle timer. */

void UiCollectionSphereZoomDetailModel(UiCollectionSphere *self)

{
  PadState *pad;
  float before;
  float after;
  
  if ((self->motionButtonMask & 2) != 0) {
    pad = (self->base).pad;
    before = self->zoom;
    if ((pad->buttons & 0x100) == 0) {
      after = before;
      if ((pad->buttons & 0x200) != 0) {
        after = before - 0.015f;
        self->zoom = after;
        if (after < 0.6f) {
          self->zoom = 0.6f;
          after = 0.6f;
        }
      }
    }
    else {
      after = before + 0.015f;
      self->zoom = after;
      if (!(after <= 1.4f)) {
        self->zoom = 1.4f;
        after = 1.4f;
      }
    }
    if (before != after) {
      self->idleTimer = 0.0f;
    }
  }
  return;
}

