// bdc 0x088ab63c ActorStageObjFindByInstanceId
#include "bdc.h"

/* Returns the first stage object in the live chain (`g_actorStageObjList->head`, `next` links)
   whose instance id (`+0x21c`) equals `instanceId`, whose dead byte `+0x281` is zero and for which
   a virtual predicate (vtable `+0x6c`, `this` adjusted by the short at `+0x68`) returns non-zero;
   NULL if none. */

typedef struct StageObjVtbl {
  u8 _pad[0x68];
  s16 adjust;
  s16 pad6a;
  int (*pred)(void *);
} StageObjVtbl;

void *ActorStageObjFindByInstanceId(u32 instanceId)
{
  CoreObject *obj;

  if (g_actorStageObjList == NULL) {
    return NULL;
  }
  for (obj = g_actorStageObjList->head; obj != NULL; obj = obj->next) {
    ActorStageObjBase *o = (ActorStageObjBase *)obj;
    const StageObjVtbl *vt = obj->vtable;

    if (o->dead != 0) {
      continue;
    }
    if (vt->pred((u8 *)obj + vt->adjust) == 0) {
      continue;
    }
    if (o->instanceId == instanceId) {
      return obj;
    }
  }
  return NULL;
}
