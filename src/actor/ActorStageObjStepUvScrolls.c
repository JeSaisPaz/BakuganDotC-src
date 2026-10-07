// bdc 0x088ac7dc ActorStageObjStepUvScrolls
#include "bdc.h"

/* Steps the four UV scroll pairs of a stage object (`+0x2c0`, `+0x2d0`, `+0x2e0`, `+0x2f0`) with
   `ActorStageObjUvScrollStep`. */

void ActorStageObjStepUvScrolls(ActorStageObjBase *self)

{
  ActorStageObjUvScrollStep((float *)self->uvScrolls);
  ActorStageObjUvScrollStep((float *)(&self->uvScrolls[0x10]));
  ActorStageObjUvScrollStep((float *)(&self->uvScrolls[0x20]));
  ActorStageObjUvScrollStep((float *)(&self->uvScrolls[0x30]));
  return;
}

