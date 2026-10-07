// bdc 0x088b31e4 ActorStageObjRecordCountType8
#include "bdc.h"

/* Counts the stage spawn records of type 8 (`+0x34`) in the record list `*0x08abd620`. Type-8 twin
   of `ActorStageObjRecordCountType4`. */

s32 ActorStageObjRecordCountType8(void)
{
  s32 count = 0;
  ActorStageObjRecord *rec;

  if (g_stageObjRecordList != (void **)0x0) {
    rec = (ActorStageObjRecord *)*g_stageObjRecordList;
    if (rec != (ActorStageObjRecord *)0x0) {
      do {
        if (rec->field32[1] == 8) {
          count = count + 1;
        }
        rec = (ActorStageObjRecord *)rec->base.next;
      } while (rec != (ActorStageObjRecord *)0x0);
    }
  }
  return count;
}
