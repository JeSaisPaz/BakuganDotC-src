// bdc 0x08937ec0 UiUnlockResultFreeModelMotion
#include "bdc.h"

/* Frees the motion file loaded for the reward model of `UiUnlockResult`: when
   a model exists (`+0x794`) releases the motion named `+0x7a8` from the Gmo motion manager
   (`GmoMotionFreeByName`). */

void UiUnlockResultFreeModelMotion(UiUnlockResult *self)

{
  void *mgr;
  
  if (self->model != (void *)0x0) {
    mgr = GmoMotionMgrGet();
    GmoMotionFreeByName(mgr,self->motionName);
  }
  return;
}

