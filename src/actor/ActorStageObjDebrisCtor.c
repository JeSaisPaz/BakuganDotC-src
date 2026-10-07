// bdc 0x088b530c ActorStageObjDebrisCtor
#include "bdc.h"

/* Constructor of the break-debris model shown when a stage object is destroyed (e.g.
   `f6_landmark01_break.gmo` from `ActorStageObjLandmarkBreak`, also used by
   `ActorCrystalBreak`): `GfxModelCtor``(obj, gmoName, 0)` and vtable `0x08af2c34`. The
   fragments are set up by `ActorStageObjDebrisLaunch`/`ActorStageObjDebrisLaunchStrong`. */

void *ActorStageObjDebrisCtor(void *obj, const char *gmoName)

{
  GfxModelCtor(obj,gmoName,0);
  ((CoreObject *)obj)->vtable = &g_actorStageObjDebrisVtable;
  return obj;
}

