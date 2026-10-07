// bdc 0x088acd0c ActorStageObjValidate
#include "bdc.h"

/* Returns `obj` if it is still a member of the live stage-object chain (`g_actorStageObjList`,
   walked through `next`) and its `dead` byte is zero, otherwise NULL. Dead members are skipped.
   Used to turn a stale object pointer held by a script into a safe handle. */

void *ActorStageObjValidate(void *obj)

{
  CoreObject *node;

  if (g_actorStageObjList != (CoreObjectList *)0x0) {
    for (node = g_actorStageObjList->head; node != (CoreObject *)0x0; node = node->next) {
      if (((ActorStageObjBase *)node)->dead == 0 && (void *)node == obj) {
        return node;
      }
    }
  }
  return (void *)0x0;
}
