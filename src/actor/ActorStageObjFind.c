// bdc 0x088ab5d8 ActorStageObjFind
#include "bdc.h"

/* Searches the live stage-object chain for an object of the given `kind` (`+0x218`) and instance id
   (`+0x21c`) and returns it, or NULL. Objects whose state byte `+0x281` is non-zero are skipped;
   `instanceId == -1` matches any instance of that kind. */

void *ActorStageObjFind(int kind, int instanceId)
{
  CoreObject *obj;

  if (g_actorStageObjList != (CoreObjectList *)0x0) {
    obj = g_actorStageObjList->head;
    while (obj != (CoreObject *)0x0) {
      ActorStageObjBase *o = (ActorStageObjBase *)obj;
      if (o->dead == 0 && o->kind == kind) {
        if (instanceId == -1 || (int)o->instanceId == instanceId) {
          return obj;
        }
      }
      obj = obj->next;
    }
  }
  return (void *)0x0;
}
