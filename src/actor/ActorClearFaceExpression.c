// bdc 0x088df748 ActorClearFaceExpression
#include "bdc.h"

/* Sets the face expression index `+0x358` to -1. */

void ActorClearFaceExpression(Actor *self)

{
  self->faceExpression = -1;
  return;
}

