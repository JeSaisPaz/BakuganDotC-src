// bdc 0x088b2734 ActorStageObjRecordUpdateAll
#include "bdc.h"

/* Per-frame step of the stage layout: tries to spawn every record of `0x08abd620`
   (`ActorStageObjRecordTrySpawn`) and updates the item spawners, stop walls and enemy spawners
   (`BtlItemSpawnerUpdateAll`, `StopWallUpdateAll`, `BtlEnemySpawnerUpdateAll`). Called by the
   battle/demo/field update loops (`BtlMainUpdateScene`, `BtlStageSpawnPlacedObjects`,
   `GameFieldUpdateWorld`, `BtlDemoUpdateObjects`). */

void ActorStageObjRecordUpdateAll(void)

{
  ActorStageObjRecord *rec;
  
  if (g_stageObjRecordList != NULL) {
    for (rec = (ActorStageObjRecord *)*g_stageObjRecordList; rec != (ActorStageObjRecord *)0x0;
        rec = (ActorStageObjRecord *)(rec->base).next) {
      ActorStageObjRecordTrySpawn(rec);
    }
  }
  BtlItemSpawnerUpdateAll();
  StopWallUpdateAll();
  BtlEnemySpawnerUpdateAll();
  return;
}

