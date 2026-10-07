// bdc 0x088a9234 ActorStageObjSystemShutdown
#include "bdc.h"

/* Deletes every model of the stage-object chain `g_actorStageObjList` (`GfxModelChainDeleteAll`), frees
   the chain head and tears down the sub-lists (`ActorStageObjRecordDestroyAll`). Called by `BtlMainTaskDtor`,
   `BtlMainTeardown`, `GameFieldDtor`. */

void ActorStageObjSystemShutdown(void)
{
  GfxModelChainDeleteAll(g_actorStageObjList->head);
  if (g_actorStageObjList != (CoreObjectList *)0x0) {
    MemLock();
    MemFree(g_actorStageObjList, (const char *)0x0, 0);
    MemUnlock();
    g_actorStageObjList = (CoreObjectList *)0x0;
  }
  ActorStageObjRecordDestroyAll();
}
