// bdc 0x088b20cc ActorStageObjRecordInitLists
#include "bdc.h"

/* Initialises the stage layout lists: allocates the spawn-record list head ``g_stageObjRecordList`` (12 bytes,
   low heap) if needed, resets the live-object counter ``g_stageObjLiveCount``, clears the last activated
   object (`GameClearLastActivatedObject`) and sets up the item-spawner, stop-wall and
   enemy-spawner lists (`BtlItemSpawnerInitList`, `StopWallEnsureList`,
   `BtlEnemySpawnerEnsureList`). Called by `ActorStageObjSystemInit` and
   `ActorStageObjRecordAdd`. */

void ActorStageObjRecordInitLists(void)

{
  bool fromLow;
  void **list;
  
  if (g_stageObjRecordList == (void **)0x0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    list = MemAlloc(sizeof(CoreObjectList),(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    g_stageObjRecordList = list;
    list[1] = (void *)0x0;
    *list = (void *)0x0;
    g_stageObjRecordList[2] = (void *)0x0;
  }
  g_stageObjLiveCount = 0;
  GameClearLastActivatedObject();
  BtlItemSpawnerInitList();
  StopWallEnsureList();
  BtlEnemySpawnerEnsureList();
  return;
}

