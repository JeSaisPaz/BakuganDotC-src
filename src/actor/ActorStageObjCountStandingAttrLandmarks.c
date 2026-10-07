// bdc 0x088b2cf4 ActorStageObjCountStandingAttrLandmarks
#include "bdc.h"

/* Counts the standing attribute landmarks (virtual `+0x64` true, `+0x281` clear) in the
   stage-object chain. Used by `BtlHudAdviceTrigger09` and `BtlMainPhaseIntro`. */

int ActorStageObjCountStandingAttrLandmarks(void)

{
  CoreObject *obj = (CoreObject *)0;
  int count;

  if (g_actorStageObjList != (CoreObjectList *)0x0) {
    obj = g_actorStageObjList->head;
  }
  count = 0;
  while (obj != (CoreObject *)0x0) {
    const VtblEntry *ent = &((const VtblEntry *)obj->vtable)[12];
    if (((int (*)(void *))ent->fn)((char *)obj + ent->delta) != 0 && ((ActorStageObjBase *)obj)->dead == 0) {
      count = count + 1;
    }
    obj = obj->next;
  }
  return count;
}
