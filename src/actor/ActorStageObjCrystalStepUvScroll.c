// bdc 0x088b4448 ActorStageObjCrystalStepUvScroll
#include "bdc.h"

/* Steps the crystal's UV scroll at `+0x380` (`ActorStageObjCrystalUvScrollStep`). Called every
   frame by `ActorStageObjCrystalUpdate`. */

void ActorStageObjCrystalStepUvScroll(ActorStageObjCrystal *self)

{
  ActorStageObjCrystalUvScrollStep((float *)self->uvScroll);
  return;
}

