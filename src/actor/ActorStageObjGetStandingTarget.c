// bdc 0x088b2f64 ActorStageObjGetStandingTarget
#include "bdc.h"

/* Returns the `index`-th standing script-created HP object (virtual `+0x6c`, at most 6 collected) or
   NULL. Used by `BtlHudDrawLayers`, `BtlMainPhaseIntro` and `UiHpGaugeDraw`. */

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
    const VtblEntry *ent = &((const VtblEntry *)obj->vtable)[13];
    if (((int (*)(void *))ent->fn)((char *)obj + ent->delta) != 0) {
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
