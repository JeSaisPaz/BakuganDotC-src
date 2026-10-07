// bdc 0x088b2208 ActorStageObjRecordSetObjScale
#include "bdc.h"

/* Finds the layout spawn record (`ActorStageObjRecordCtor`) with kind `kind` (`+0x32`) and its
   live stage object (`obj+0x154 == rec`), then sets that object's uniform scale to `scale`, moves
   it to the record position and lowers it by `bounds.minY * scale` (`ActorStageObjGetBounds`).
   Called by `UiScreen390MainPhase`. */

void ActorStageObjRecordSetObjScale(float scale, int kind)
{
  ActorStageObjRecord *rec;
  ActorStageObjBase *obj;
  float *pos;
  float y;
  float *bounds;

  if (g_stageObjRecordList == (void **)0x0) {
    return;
  }
  rec = (ActorStageObjRecord *)*g_stageObjRecordList;
  if (rec == (ActorStageObjRecord *)0x0) {
    return;
  }
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
      obj->base.scale[0] = scale;
      obj->base.scale[1] = scale;
      obj->base.scale[2] = scale;
      obj->base.scale[3] = 0.0f;
      obj->base.pos[0] = rec->pos[0];
      obj->base.pos[1] = rec->pos[1];
      obj->base.pos[2] = rec->pos[2];
      obj->base.pos[3] = rec->pos[3];
      pos = &obj->base.pos[1];
      y = *pos;
      bounds = ActorStageObjGetBounds(obj);
      *pos = y - bounds[1] * scale;
      return;
    }
    obj = (ActorStageObjBase *)obj->base.base.next;
  }
}
