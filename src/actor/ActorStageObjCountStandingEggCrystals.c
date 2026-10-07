// bdc 0x088b30f4 ActorStageObjCountStandingEggCrystals
#include "bdc.h"

/* Counts the standing egg crystals (virtual `+0x74` true, `+0x281` clear). Used by
   `ActorCrystalUpdateSpellCast`, `BtlBakuganRunBallEntry` and `ActorStageObjEggCrystalSummon`
   (camera focus on the first one). */

int ActorStageObjCountStandingEggCrystals(void)

{
  CoreObject *obj = (CoreObject *)0;
  int count;

  if (g_actorStageObjList != (CoreObjectList *)0x0) {
    obj = g_actorStageObjList->head;
  }
  count = 0;
  while (obj != (CoreObject *)0x0) {
    const VtblEntry *ent = &((const VtblEntry *)obj->vtable)[14];
    if (((int (*)(void *))ent->fn)((char *)obj + ent->delta) != 0 && ((ActorStageObjBase *)obj)->dead == 0) {
      count = count + 1;
    }
    obj = obj->next;
  }
  return count;
}
