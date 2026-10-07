// bdc 0x088f773c GameQuestCamPathModeUpdateTarget
#include "bdc.h"

/* Slot 5 of the path camera mode: target update (`GameQuestCamModeSyncNodeThunk`), behind check,
   smoothing, segment attachment (`GameQuestCamAttachToPath`) and, when attached, the eye
   placement (`GameQuestCamFollowPath`). */

void GameQuestCamPathModeUpdateTarget(float dt, GameQuestCamPathMode *self)

{
  GameQuestCamModeSyncNodeThunk(&self->base);
  GameQuestCamPathModeCheckBehind(self);
  GameQuestCamPathModeSmooth(dt,self);
  GameQuestCamAttachToPath(self);
  if (self->segment != (void *)0x0) {
    GameQuestCamFollowPath(self);
  }
  return;
}

