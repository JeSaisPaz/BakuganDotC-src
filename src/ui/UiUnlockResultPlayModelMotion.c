// bdc 0x0893b720 UiUnlockResultPlayModelMotion
#include "bdc.h"

/* For reward kind `+0x5ee` 5 (Maxus part model) on `UiUnlockResult`: derives
   the motion name from the model file name (model `name`, up to the first `.`), keeps it in
   `motionName`, loads `"%s_motion.gmo"` (`GmoMotionLoadFile`), enables motion on the model and
   selects the motion at frame 0.2 (`GfxModelPlayMotionByName`, no loop), then sets the motion speed to 1 and
   updates the model once through its vtable. Always resets the bob timer `modelTimer` first; does
   nothing else when there is no model or the reward kind is not 5. */

void UiUnlockResultPlayModelMotion(UiUnlockResult *self)

{
  char *motion;
  void *mgr;
  const GfxModelVtable *vt;
  char name[64];
  char file[64];

  self->modelTimer = 0.0f;
  if ((self->model != NULL) && (self->rewardKind == 5)) {
    strncpy(name, ((GfxModel *)self->model)->name, 0x40);
    motion = strtok(name, ".");
    strncpy(self->motionName, motion, 0x40);
    sprintf(file, "%s_motion.gmo", motion);
    mgr = GmoMotionMgrGet();
    GmoMotionLoadFile(mgr, file);
    GfxModelEnableMotion((GfxModel *)self->model);
    GfxModelPlayMotionByName(0.2f, (GfxModel *)self->model, motion, false);
    vt = (const GfxModelVtable *)((GfxModel *)self->model)->base.vtable;
    vt->setMotionSpeed((u8 *)self->model + vt->setMotionSpeedAdjust, 1.0f);
    vt = (const GfxModelVtable *)((GfxModel *)self->model)->base.vtable;
    vt->update((u8 *)self->model + vt->updateAdjust);
  }
}
