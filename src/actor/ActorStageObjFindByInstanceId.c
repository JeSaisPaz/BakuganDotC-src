// bdc 0x088ab63c ActorStageObjFindByInstanceId
#include "bdc.h"

/* Returns the first stage object in the live chain (`g_actorStageObjList->head`, `next` links)
   whose instance id (`+0x21c`) equals `instanceId`, whose dead byte `+0x281` is zero and for which
   a virtual predicate (vtable `+0x6c`, `this` adjusted by the short at `+0x68`) returns non-zero;
   NULL if none. */

void *ActorStageObjFindByInstanceId(u32 instanceId)
{
  CoreObject *obj;

  if (g_actorStageObjList == NULL) {
    return NULL;
  }
  for (obj = g_actorStageObjList->head; obj != NULL; obj = obj->next) {
    ActorStageObjBase *o = (ActorStageObjBase *)obj;
    const VtblEntry *pred = &((const VtblEntry *)obj->vtable)[13];

    if (o->dead != 0) {
      continue;
    }
    if (((int (*)(void *))pred->fn)((u8 *)obj + pred->delta) == 0) {
      continue;
    }
    if (o->instanceId == instanceId) {
      return obj;
    }
  }
  return NULL;
}
