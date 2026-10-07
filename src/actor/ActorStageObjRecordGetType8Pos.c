// bdc 0x088b3174 ActorStageObjRecordGetType8Pos
#include "bdc.h"

/* Copies the position (`+0x20..+0x2c`) of the stage spawn record of type 8 (`+0x34`) and id `id` (`+0x30`) from the record list `*0x08abd620` (`ActorStageObjRecordAdd`) into `out`; zeros when the list is missing or no record matches. Type-8 twin of `ActorStageObjRecordGetType4Pos`. */

void ActorStageObjRecordGetType8Pos(float *out, s32 id)
{
  ActorStageObjRecord *rec;

  if (g_stageObjRecordList != (void **)0x0) {
    rec = (ActorStageObjRecord *)*g_stageObjRecordList;
    while (rec != (ActorStageObjRecord *)0x0) {
      if (rec->field32[1] == 8 && rec->field30 == id) {
        out[0] = rec->pos[0];
        out[1] = rec->pos[1];
        out[2] = rec->pos[2];
        out[3] = rec->pos[3];
        return;
      }
      rec = (ActorStageObjRecord *)rec->base.next;
    }
  }
  out[2] = 0.0f;
  out[1] = 0.0f;
  out[0] = 0.0f;
  out[3] = 0.0f;
}
