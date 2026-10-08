// bdc 0x088a1f50 ActorStageObjStopEffect
#include "bdc.h"

/* Ends the looping effect of a stage-object subclass: asks effect id 99 attached to `obj->orbPos`
   (`obj+0x390`) to switch to state 2 (`GfxEffectSetStateAttached`), stops the effect with id
   `0x162` attached to the same position (`GfxEffectStopAttached`) on the effect manager
   `g_btlUnitEffectMgr`, and clears the effect handle `obj->effect` (`+0x328`). */

void ActorStageObjStopEffect(ActorStageObjLandmark *obj)

{
  GfxEffectSetStateAttached(g_btlUnitEffectMgr, 99, obj->orbPos, 2);
  GfxEffectStopAttached(g_btlUnitEffectMgr, 0x162, obj->orbPos);
  obj->effect = NULL;
}
