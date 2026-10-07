// bdc 0x089dfe04 GfxModelSelectMotion
#include "bdc.h"

/* Selects motion `index` on a GMO model object: clears `motionEnded`; for index -1 returns false.
   Without `registryMotions` a valid index (`< motionCount`) is handed to `GmoModelSelectMotion`
   on the model's GMO data and stored in `motionSlot` (an out-of-range index is ignored but still
   returns true); with `registryMotions` the next of 3 motion slots (`motionSlot = (motionSlot + 1) % 3`)
   of the GMO data's `GmoMotionInfo` table is overwritten with the registry motion's data
   (`GmoMotionGetDataByIndex`) and `motionIndex` records the registry index (both only when the
   registry motion exists), then the slot is selected with `GmoModelSelectMotion`. Returns true. */

bool GfxModelSelectMotion(float frame, GfxModel *self, s32 index)
{
  GmoMotionInfo *info;

  self->motionEnded = 0;
  if (index == -1) {
    return false;
  }
  if (self->registryMotions == 0) {
    if (index < self->motionCount) {
      GmoModelSelectMotion(frame, self->data, index);
      self->motionSlot = (u8)index;
    }
  }
  else {
    self->motionSlot = (u8)((self->motionSlot + 1) % 3);
    info = GmoMotionGetDataByIndex(GmoMotionMgrGet(), index);
    if (info != NULL) {
      self->motionIndex = index;
      memcpy(&((GmoMotionInfo *)self->data->motions)[self->motionSlot], info,
             sizeof(GmoMotionInfo));
    }
    GmoModelSelectMotion(frame, self->data, self->motionSlot);
  }
  return true;
}
