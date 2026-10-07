// bdc 0x088b27c8 ActorStageObjRecordDestroyAll
#include "bdc.h"

/* Destroys all spawn records of `g_stageObjRecordList` (virtual destructor, flags 3), frees the
   head and tears down the item spawners, stop walls and enemy spawners
   (`BtlItemSpawnerDestroyAll`, `StopWallDestroyAll`, `BtlEnemySpawnerDestroyAll`). Called by
   `ActorStageObjSystemShutdown`. */

void ActorStageObjRecordDestroyAll(void)

{
  CoreObject *obj;
  CoreObject *next;

  if (g_stageObjRecordList != (void **)0x0) {
    obj = *(CoreObject **)g_stageObjRecordList;
    if (obj != (CoreObject *)0x0) {
      next = obj->next;
      while (1) {
        if (obj != (CoreObject *)0x0) {
          const VtblEntry *ent = &((const VtblEntry *)obj->vtable)[1];
          ((void (*)(void *, int))ent->fn)((char *)obj + ent->delta, 3);
        }
        if (next == (CoreObject *)0x0) break;
        obj = next;
        next = next->next;
      }
    }
    if (g_stageObjRecordList != (void **)0x0) {
      MemLock();
      MemFree(g_stageObjRecordList, (const char *)0, 0);
      MemUnlock();
      g_stageObjRecordList = (void **)0;
    }
  }
  BtlItemSpawnerDestroyAll();
  StopWallDestroyAll();
  BtlEnemySpawnerDestroyAll();
}
