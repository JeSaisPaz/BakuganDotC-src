// bdc 0x088aacfc ActorStageObjNeedsCollider
#include "bdc.h"

/* `ActorStageObjCategoryNeedsGround` for a stack copy of the object's position and its `category`. */

int ActorStageObjNeedsCollider(ActorStageObjBase *self)
{
  float pos[4] __attribute__((aligned(16)));

  pos[0] = self->base.pos[0];
  pos[1] = self->base.pos[1];
  pos[2] = self->base.pos[2];
  pos[3] = self->base.pos[3];
  return ActorStageObjCategoryNeedsGround(pos, self->category);
}
