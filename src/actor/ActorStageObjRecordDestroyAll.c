// bdc 0x088b27c8 ActorStageObjRecordDestroyAll
#include "bdc.h"

/* Destroys all spawn records of `g_stageObjRecordList` (virtual destructor, flags 3), frees the
   head and tears down the item spawners, stop walls and enemy spawners
   (`BtlItemSpawnerDestroyAll`, `StopWallDestroyAll`, `BtlEnemySpawnerDestroyAll`). Called by
   `ActorStageObjSystemShutdown`. */

typedef struct RecordVirtEntry {
  short adj;
  short pad;
  void (*fn)(void *self, int flags);
} RecordVirtEntry;

typedef struct RecordVtable {
  RecordVirtEntry slot[2];
} RecordVtable;

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
          const RecordVirtEntry *ent = (const RecordVirtEntry *)&((const RecordVtable *)obj->vtable)->slot[1];
          ent->fn((char *)obj + ent->adj, 3);
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
