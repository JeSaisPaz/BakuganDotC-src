// bdc 0x088b33d0 ActorStageObjReleaseRecord
#include "bdc.h"

/* Releases a stage-object layout spawn record: when its object link `rec+0x18` is set, clears it
   and decrements the live-object counter `0x08b00b90`. Stage objects keep their record at `+0x154`
   (set by `ActorStageObjSetRecord`) and release it when destroyed. */

void ActorStageObjReleaseRecord(void *rec)

{
  ActorStageObjRecord *r = (ActorStageObjRecord *)rec;

  if (r->liveCount != 0) {
    r->liveCount = 0;
    g_stageObjLiveCount = g_stageObjLiveCount - 1;
  }
}
