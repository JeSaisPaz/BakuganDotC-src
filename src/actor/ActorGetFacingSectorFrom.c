// bdc 0x088df914 ActorGetFacingSectorFrom
#include "bdc.h"

/* Computes the XZ angle from `pos` to the actor and classifies it with `ActorGetFacingSector` (1
   = the actor faces `pos`). Used by `ActorPlayerFindTalkTarget`. */

s32 ActorGetFacingSectorFrom(Actor *actor, const float *pos)
{
  float angle;

  angle = atan2f(actor->base.pos[2] - pos[2], actor->base.pos[0] - pos[0]);
  return ActorGetFacingSector(angle, actor);
}
