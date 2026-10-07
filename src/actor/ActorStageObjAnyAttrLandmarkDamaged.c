// bdc 0x088b2d74 ActorStageObjAnyAttrLandmarkDamaged
#include "bdc.h"

/* Returns 1 when any attribute landmark (virtual `+0x64`) has lost HP (`maxHp +0x204 > hp +0x200`),
   else 0. Called by `BtlHudAdviceTutorialIntro`. */

int ActorStageObjAnyAttrLandmarkDamaged(void)
{
  CoreObject *obj = (CoreObject *)0x0;

  if (g_actorStageObjList != (CoreObjectList *)0x0) {
    obj = g_actorStageObjList->head;
  }
  while (obj != (CoreObject *)0x0) {
    const VtblEntry *ent = &((const VtblEntry *)obj->vtable)[12];

    if (((int (*)(void *))ent->fn)((char *)obj + ent->delta) != 0) {
      ActorStageObjBase *o = (ActorStageObjBase *)obj;

      if (!((float)o->maxHp - (float)o->hp <= 0.0f)) {
        return 1;
      }
    }
    obj = obj->next;
  }
  return 0;
}
