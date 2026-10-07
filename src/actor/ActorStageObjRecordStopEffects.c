// bdc 0x088b2ae4 ActorStageObjRecordStopEffects
#include "bdc.h"

/* Stops every effect owned by the first spawn record with kind `kind` on both effect managers
   (`0x08ac5c70`, `0x08abd5b0`; `GfxEffectStopOwned`). Called by `UiScreen390ClearTerminalEffect`. */

void ActorStageObjRecordStopEffects(int kind)

{
  ActorStageObjRecord *owner;

  if ((g_stageObjRecordList != (void **)0x0) &&
      (owner = (ActorStageObjRecord *)*g_stageObjRecordList, owner != (ActorStageObjRecord *)0x0)) {
    while (owner->field32[0] != kind) {
      owner = (ActorStageObjRecord *)owner->base.next;
      if (owner == (ActorStageObjRecord *)0x0) {
        return;
      }
    }
    GfxEffectStopOwned(g_worldEffectMgr,-1,owner);
    if (g_btlUnitEffectMgr != (GfxEffectMgr *)0x0) {
      GfxEffectStopOwned(g_btlUnitEffectMgr,-1,owner);
    }
  }
  return;
}
