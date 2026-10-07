// bdc 0x088b3290 ActorStageObjRecordCountType4
#include "bdc.h"

/* Counts the type-4 spawn records (crystal points, see `ActorStageObjRecordGetType4Pos`). Called
   by `ActorCrystalSpawnTaskStep` and `ActorCrystalPickFreeSpawnPoint`. */

int ActorStageObjRecordCountType4(void)
{
  int count = 0;
  ActorStageObjRecord *rec;

  if (g_stageObjRecordList != (void **)0x0) {
    rec = (ActorStageObjRecord *)*g_stageObjRecordList;
    if (rec != (ActorStageObjRecord *)0x0) {
      do {
        if (rec->field32[1] == 4) {
          count = count + 1;
        }
        rec = (ActorStageObjRecord *)rec->base.next;
      } while (rec != (ActorStageObjRecord *)0x0);
    }
  }
  return count;
}
