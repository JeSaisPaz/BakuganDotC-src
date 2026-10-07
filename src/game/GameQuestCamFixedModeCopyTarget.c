// bdc 0x088f67b8 GameQuestCamFixedModeCopyTarget
#include "bdc.h"

/* Vtable `0x08af43d4` slot 5 of the fixed-camera mode: copies the entry's fixed point
   (`entry->pos`) to the spring goal. */

void GameQuestCamFixedModeCopyTarget(GameQuestCamFixedMode *self)

{
  self->base.base.goal = self->entry->pos;
}
