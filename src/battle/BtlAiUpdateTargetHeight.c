// bdc 0x0889320c BtlAiUpdateTargetHeight
#include "bdc.h"

/* Stores the height of the current target above the owner in targetHeight (target Y minus the
   owner's ground height), 0 when there is no target. Read by condition kind 0x29 of
   `BtlAiEvalCondition`. */

void BtlAiUpdateTargetHeight(BtlAi *self)
{
  self->targetHeight = 0.0f;
  if (self->target != NULL) {
    self->targetHeight = self->target->base.pos[1] - self->owner->groundY;
  }
}
