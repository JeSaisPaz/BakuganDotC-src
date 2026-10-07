// bdc 0x089e0830 GfxModelEnableMotion
#include "bdc.h"

/* Enables motion playback on a model once (`+0x13d`): when the global switch `0x08ac5c64` is on,
   uses the model's work area as GMO bump region and either marks it for the global motion registry
   (`+0x13c`, when it has own motions) or, with no motion chunks, allocates three motion slots
   (`GmoCreateMotionArray(3)`, count 3) for registry motions. */

void GfxModelEnableMotion(GfxModel *self)

{
  void *motions;
  int count;

  if ((self->motionEnabled == '\0') && (self->motionEnabled = '\x01', g_gmoMotionEnabled != '\0')) {
    if (self->work == (void *)0x0) {
      GmoSetBumpRegion(self->workDefault,&self->bumpUsed,0x2000);
      count = self->motionCount;
    }
    else {
      GmoSetBumpRegion(self->work,&self->bumpUsed,self->workSize);
      count = self->motionCount;
    }
    if (0 < count) {
      self->registryMotions = '\x01';
    }
    if (count == 0) {
      motions = GmoCreateMotionArray(3);
      self->data->motions = motions;
      self->data->motionCount = 3;
      self->registryMotions = '\x01';
    }
    GmoSetBumpRegion((void *)0x0,(u32 *)0x0,0x2000);
  }
  return;
}

