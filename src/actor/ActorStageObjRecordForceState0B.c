// bdc 0x088b2bf4 ActorStageObjRecordForceState0B
#include "bdc.h"

/* Finds the live stage object of the first spawn record with kind `kind` and puts it into state 0xb
   (`+0x304`, sub-step `+0x1fc` = 0) of the shared state machine. Called by
   `UiScreen390MainPhase`. */

void ActorStageObjRecordForceState0B(int kind)

{
  ActorStageObjRecord *rec;
  ActorStageObjBase *obj;

  if ((g_stageObjRecordList != (void **)0x0) &&
      (rec = (ActorStageObjRecord *)*g_stageObjRecordList, rec != (ActorStageObjRecord *)0x0)) {
    while (rec->field32[0] != kind) {
      rec = (ActorStageObjRecord *)rec->base.next;
      if (rec == (ActorStageObjRecord *)0x0) {
        return;
      }
    }
    obj = (ActorStageObjBase *)0x0;
    if (g_actorStageObjList != (CoreObjectList *)0x0) {
      obj = (ActorStageObjBase *)g_actorStageObjList->head;
    }
    while (obj != (ActorStageObjBase *)0x0) {
      if (obj->record == rec) {
        obj->state = 0xb;
        obj->step = 0;
        return;
      }
      obj = (ActorStageObjBase *)obj->base.base.next;
    }
  }
  return;
}
