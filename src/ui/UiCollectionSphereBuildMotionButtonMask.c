// bdc 0x0897de20 UiCollectionSphereBuildMotionButtonMask
#include "bdc.h"

/* Builds the motion-view button mask `+0x1134` of `UiCollectionSphere`
   (bit0 rotate, bit1 zoom, bit2/3 pop-out motion) for the selected entry; special entries get fewer
   buttons. */

void UiCollectionSphereBuildMotionButtonMask(UiCollectionSphere *self)

{
  int bit;

  self->motionButtonMask = 0;
  for (bit = 0; bit < 4; bit++) {
    int enabled = 1;
    if (self->category < 0 || self->category > 1) {
      if (UiCollectionSphereGetPageKind(self,self->page) == 2 && bit == 3) {
        enabled = 0;
      }
    }
    if (enabled != 0) {
      self->motionButtonMask = self->motionButtonMask | (u8)(1 << bit);
    }
  }
  return;
}
