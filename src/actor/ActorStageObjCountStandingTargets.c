// bdc 0x088b2c74 ActorStageObjCountStandingTargets
#include "bdc.h"

/* Counts the standing (`+0x281` clear) script-created HP objects (virtual `+0x6c` true) in the
   stage-object chain `0x08abd5bc`. Used by the battle intro (`BtlMainCheckPlayerDefeated`, `BtlMainPhaseIntro`)
   and `ActorStageObjUpdateScripted`. */

/* vtable slot: this-adjust + function pointer */
typedef struct StageObjVirtEntry {
  short adj;
  short pad;
  int (*fn)(void *self);
} StageObjVirtEntry;

typedef struct StageObjVtable {
  StageObjVirtEntry slot[15];
} StageObjVtable;

int ActorStageObjCountStandingTargets(void)

{
  CoreObject *obj = (CoreObject *)0;
  int count;

  if (g_actorStageObjList != (CoreObjectList *)0x0) {
    obj = g_actorStageObjList->head;
  }
  count = 0;
  while (obj != (CoreObject *)0x0) {
    const StageObjVirtEntry *ent = (const StageObjVirtEntry *)&((const StageObjVtable *)obj->vtable)->slot[13];
    if (ent->fn((char *)obj + ent->adj) != 0 && ((ActorStageObjBase *)obj)->dead == 0) {
      count = count + 1;
    }
    obj = obj->next;
  }
  return count;
}
