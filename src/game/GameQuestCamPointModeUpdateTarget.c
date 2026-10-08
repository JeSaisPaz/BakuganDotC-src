// bdc 0x088f8644 GameQuestCamPointModeUpdateTarget
#include "bdc.h"

/* Slot 5 of the point camera mode: target update (`GameQuestCamModeSyncNodeThunk`), behind check,
   smoothing along `+0x120` (`GameQuestCamModeSmooth`) and the follow offset
   (`GameQuestCamUpdateFollowOffset`). */

void GameQuestCamPointModeUpdateTarget(float dt, GameQuestCamPointMode *self)

{
  GameQuestCamModeSyncNodeThunk(&self->base);
  GameQuestCamPointModeCheckBehind(self);
  GameQuestCamModeSmooth(dt,&self->base,&(self->viewDir).x,self->entry->leadScale);
  GameQuestCamUpdateFollowOffset(self);
  return;
}

