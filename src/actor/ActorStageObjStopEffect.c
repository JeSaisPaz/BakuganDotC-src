// bdc 0x088a1f50 ActorStageObjStopEffect
#include "bdc.h"

/* Ends the looping effect of a stage-object subclass: asks effect id 99 attached to `&obj->state`
   (`obj+0x390`) to switch to state 2 (`GfxEffectSetStateAttached`), stops the effect with id
   `0x162` attached to the same position (`GfxEffectStopAttached`) on the effect manager
   `g_btlUnitEffectMgr`, and clears the effect handle `obj->step` (`+0x328`). */

void ActorStageObjStopEffect(ActorStageObjCrystal *obj)

{
  GfxEffectSetStateAttached(g_btlUnitEffectMgr, 99, &obj->state, 2);
  GfxEffectStopAttached(g_btlUnitEffectMgr, 0x162, &obj->state);
  obj->step = 0;
}
