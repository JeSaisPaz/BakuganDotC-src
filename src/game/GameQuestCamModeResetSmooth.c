// bdc 0x088fd404 GameQuestCamModeResetSmooth
#include "bdc.h"

/* Resets the smoothing state of a quest camera mode: clears `leadFilter` and `lead`, and sets the
   first-call flag so `GameQuestCamModeSmooth` recaptures the target position. */

void GameQuestCamModeResetSmooth(GameQuestCamModeBase *self)

{
  self->leadFilter = 0.0f;
  self->lead.x = 0.0f;
  self->lead.y = 0.0f;
  self->lead.z = 0.0f;
  self->lead.w = 0.0f;
  self->firstCall = 1;
}
