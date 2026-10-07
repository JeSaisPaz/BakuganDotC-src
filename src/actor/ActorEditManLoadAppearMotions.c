// bdc 0x088deab8 ActorEditManLoadAppearMotions
#include "bdc.h"

/* Loads the edit-man appearance motions `12_editm_braw.gmo` and `12_editm_dir_app.gmo` into the
   motion manager and stores their indices in motion slots 0x2f and 0x30 of the actor
   (`self->motionSlots[0x2f]` / `[0x30]`). Used by `BtlAppearDemoStateLoad`. */

void ActorEditManLoadAppearMotions(Actor *self)
{
  GmoMotionLoadFile(GmoMotionMgrGet(), "12_editm_braw.gmo");
  GmoMotionLoadFile(GmoMotionMgrGet(), "12_editm_dir_app.gmo");
  self->motionSlots[0x2f] = (s16)GmoMotionIndexOfName(GmoMotionMgrGet(), "12_editm_braw");
  self->motionSlots[0x30] = (s16)GmoMotionIndexOfName(GmoMotionMgrGet(), "12_editm_dir_app");
}
