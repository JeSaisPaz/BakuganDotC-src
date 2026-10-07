// bdc 0x088acb9c ActorStageObjNotifyTargeted
#include "bdc.h"

/* Calls virtual `+0x44` with `arg` on every stage object flagged as the local player's target
   (`+0x288`, set by `ActorStageObjMarkTargeted`). Called by `BtlMainDrawScene` and
   `BtlMainDrawCutIn`. */

void ActorStageObjNotifyTargeted(void *arg)
{
  CoreObject *obj;

  if (g_actorStageObjList != (CoreObjectList *)0) {
    for (obj = g_actorStageObjList->head; obj != (CoreObject *)0; obj = obj->next) {
      if (((ActorStageObjBase *)obj)->targeted != 0) {
        const VtblEntry *fn = &((const VtblEntry *)obj->vtable)[8];
        ((void (*)(void *, void *))fn->fn)((u8 *)obj + fn->delta, arg);
      }
    }
  }
}
