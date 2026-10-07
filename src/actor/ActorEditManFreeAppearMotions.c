// bdc 0x088deb40 ActorEditManFreeAppearMotions
#include "bdc.h"

/* Frees the motions `12_editm_braw` and `12_editm_dir_app` from the motion manager
   (`GmoMotionFreeByName`); counterpart of `ActorEditManLoadAppearMotions`. */

void ActorEditManFreeAppearMotions(void)
{
  GmoMotionFreeByName(GmoMotionMgrGet(), "12_editm_braw");
  GmoMotionFreeByName(GmoMotionMgrGet(), "12_editm_dir_app");
}
