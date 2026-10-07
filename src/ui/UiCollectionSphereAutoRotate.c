// bdc 0x0897e6e8 UiCollectionSphereAutoRotate
#include "bdc.h"

/* Counts idle frames in the motion view of `UiCollectionSphere`
   (`+0x1374`) and, after 180, turns the model slowly (yaw +0.015 per frame). */

void UiCollectionSphereAutoRotate(UiCollectionSphere *self)

{
  if (self->idleTimer < 180.0f) {
    self->idleTimer = self->idleTimer + 1.0f;
    return;
  }
  self->yaw = self->yaw + 0.015f;
  return;
}

