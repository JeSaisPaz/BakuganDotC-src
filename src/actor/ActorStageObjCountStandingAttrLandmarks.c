// bdc 0x088b2cf4 ActorStageObjCountStandingAttrLandmarks
#include "bdc.h"

/* Counts the standing attribute landmarks (virtual `+0x64` true, `+0x281` clear) in the
   stage-object chain. Used by `BtlHudAdviceTrigger09` and `BtlMainPhaseIntro`. */

/* vtable slot: this-adjust + function pointer */
typedef struct StageObjVirtEntry {
  short adj;
  short pad;
  int (*fn)(void *self);
} StageObjVirtEntry;

typedef struct StageObjVtable {
  StageObjVirtEntry slot[15];
} StageObjVtable;

int ActorStageObjCountStandingAttrLandmarks(void)

{
  CoreObject *obj = (CoreObject *)0;
  int count;

  if (g_actorStageObjList != (CoreObjectList *)0x0) {
    obj = g_actorStageObjList->head;
  }
  count = 0;
  while (obj != (CoreObject *)0x0) {
    const StageObjVirtEntry *ent = (const StageObjVirtEntry *)&((const StageObjVtable *)obj->vtable)->slot[12];
    if (ent->fn((char *)obj + ent->adj) != 0 && ((ActorStageObjBase *)obj)->dead == 0) {
      count = count + 1;
    }
    obj = obj->next;
  }
  return count;
}
