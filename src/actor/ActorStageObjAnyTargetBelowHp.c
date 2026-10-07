// bdc 0x088b2eac ActorStageObjAnyTargetBelowHp
#include "bdc.h"

/* Returns 1 when a standing script-created HP object (virtual `+0x6c`) has `hp / maxHp < ratio`,
   else 0. Called by `BtlHudAdviceTrigger07` and `BtlHudAdviceTrigger08` (talk/event triggers). */

int ActorStageObjAnyTargetBelowHp(float ratio)
{
  CoreObject *obj = (CoreObject *)0x0;

  if (g_actorStageObjList != (CoreObjectList *)0x0) {
    obj = g_actorStageObjList->head;
  }
  while (obj != (CoreObject *)0x0) {
    const VtblEntry *ent = &((const VtblEntry *)obj->vtable)[13];

    if (((int (*)(void *))ent->fn)((char *)obj + ent->delta) != 0) {
      ActorStageObjBase *o = (ActorStageObjBase *)obj;

      if (o->dead == 0) {
        if ((float)o->hp / (float)o->maxHp < ratio) {
          return 1;
        }
      }
    }
    obj = obj->next;
  }
  return 0;
}
