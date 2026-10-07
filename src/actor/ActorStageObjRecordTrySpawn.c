// bdc 0x088b26f4 ActorStageObjRecordTrySpawn
#include "bdc.h"

/* Spawns the record's objects (`ActorStageObjRecordSpawn`) unless the record is done (`+0x3a`) or
   already has live objects (`+0x18`, set to the returned count). */

void ActorStageObjRecordTrySpawn(ActorStageObjRecord *rec)
{
  if ((rec->doneFlags[0] == 0) && (rec->liveCount == 0)) {
    rec->liveCount = ActorStageObjRecordSpawn(rec);
  }
}
