// bdc 0x088a75ac ActorStageObjAttrLandmarkSetActivated
#include "bdc.h"

/* Activation step used by stage-object state 9 (`ActorStageObjState09Update`): spawns effect 0x4c
   at the model (`GfxEffectSpawn` on `0x08ac5c70`), sets the companion's mode `+0x33c → +0x170`
   to 4 and the landmark mode `+0x3b4` to 3. */

void ActorStageObjAttrLandmarkSetActivated(ActorStageObjAttrLandmark *self)

{
  GfxEffectSpawn(g_worldEffectMgr,0x4c,((self->base).base.data)->rootMatrix + 0xc);
  if (self->companion != (void *)0x0) {
    self->companion->companionMode = 4;
  }
  self->mode = 3;
  return;
}

