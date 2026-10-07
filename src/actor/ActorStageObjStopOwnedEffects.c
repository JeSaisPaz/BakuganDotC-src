// bdc 0x088a1f28 ActorStageObjStopOwnedEffects
#include "bdc.h"

/* Stops every effect on the effect manager `0x08abd5b0` owned by `obj` (`GfxEffectStopOwned(mgr,
   -1, obj)`). Used by `ActorStageObjLandmarkDtor` and `ActorStageObjLandmarkBreak`. */

void ActorStageObjStopOwnedEffects(ActorStageObjBase *self)

{
  GfxEffectStopOwned(g_btlUnitEffectMgr, -1, self);
}
