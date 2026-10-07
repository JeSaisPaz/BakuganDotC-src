// bdc 0x088b2b64 ActorStageObjRecordSpawnEffect
#include "bdc.h"

/* Replaces the effect of the first spawn record with kind `kind`: stops its owned effects and
   spawns effect `effectId` at the record position owned by the record (`GfxEffectSpawn` on
   `0x08ac5c70`). Called by `UiScreen390RestoreTerminalEffect`. */

void ActorStageObjRecordSpawnEffect(int kind, int effectId)
{
  ActorStageObjRecord *rec;
  GfxEffect *effect;

  if (g_stageObjRecordList == (void **)0x0) {
    return;
  }
  rec = (ActorStageObjRecord *)*g_stageObjRecordList;
  while (rec != (ActorStageObjRecord *)0x0) {
    if (rec->field32[0] == kind) {
      GfxEffectStopOwned(g_worldEffectMgr, -1, rec);
      effect = (GfxEffect *)GfxEffectSpawn(g_worldEffectMgr, effectId, rec->pos);
      effect->ownerBakugan = rec;
      effect->ownerId = rec->base.id;
      return;
    }
    rec = (ActorStageObjRecord *)rec->base.next;
  }
}
