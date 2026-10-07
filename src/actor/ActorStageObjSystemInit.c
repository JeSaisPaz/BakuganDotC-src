// bdc 0x088a9194 ActorStageObjSystemInit
#include "bdc.h"

/* Initialises the stage-object system: allocates the stage-object chain head `0x08abd5bc` (12
   bytes, low heap) if needed, runs the sub-list setup `ActorStageObjRecordInitLists` (item
   spawners, enemy spawners, …) and clears the frame counter `0x08abd5b4` and the creation counter
   `0x08abd5b8`. Called by `GameFieldPhaseLoad`, `BtlDemoStateLoad`, `BtlAppearDemoStateLoad`.
    */

void ActorStageObjSystemInit(void)
{
  if (g_actorStageObjList == (CoreObjectList *)0x0) {
    bool fromLow;
    CoreObjectList *list;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    list = (CoreObjectList *)MemAlloc(0xc, (const char *)0x0, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    g_actorStageObjList = list;
    list->tail = (CoreObject *)0x0;
    g_actorStageObjList->head = (CoreObject *)0x0;
    g_actorStageObjList->count = 0;
  }
  ActorStageObjRecordInitLists();
  g_actorStageObjFrame = 0;
  g_actorStageObjCreateCount = 0;
}
