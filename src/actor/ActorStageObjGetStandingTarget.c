// bdc 0x088b2f64 ActorStageObjGetStandingTarget
#include "bdc.h"

/* Returns the `index`-th standing script-created HP object (virtual `+0x6c`, at most 6 collected) or
   NULL. Used by `BtlHudDrawLayers`, `BtlMainPhaseIntro` and `UiHpGaugeDraw`. */

typedef struct StageObjVirtEntry {
  short adj;
  short pad;
  int (*fn)(void *self);
} StageObjVirtEntry;

typedef struct StageObjVtable {
  StageObjVirtEntry slot[15];
} StageObjVtable;

void *ActorStageObjGetStandingTarget(int index)

{
  CoreObject *found[6];
  CoreObject *obj = (CoreObject *)0;
  int n;

  if (g_actorStageObjList != (CoreObjectList *)0x0) {
    obj = g_actorStageObjList->head;
  }
  n = 0;
  while (obj != (CoreObject *)0x0) {
    const StageObjVirtEntry *ent = (const StageObjVirtEntry *)&((const StageObjVtable *)obj->vtable)->slot[13];
    if (ent->fn((char *)obj + ent->adj) != 0) {
      if (((ActorStageObjBase *)obj)->dead == 0 && n < 6) {
        found[n] = obj;
        n = n + 1;
      }
    }
    obj = obj->next;
  }
  if (index < n) {
    return found[index];
  }
  return (void *)0;
}
