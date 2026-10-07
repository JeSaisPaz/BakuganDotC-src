// bdc 0x088acc10 ActorStageObjMarkTargeted
#include "bdc.h"

/* For the local player's Bakugan (`BtlBakuganIsLocalPlayer`) with a target
   (`BtlBakuganGetTarget`), flags (`targeted`) the stage objects whose companion unit `+800` is that
   target (only objects for which virtual `+0x74` or `+0x7c` holds), clearing the flag on all
   others. Called by `BtlMainStartCutIn`. */

void ActorStageObjMarkTargeted(void *unit)

{
  ActorStageObj *obj;

  if (g_actorStageObjList == (CoreObjectList *)0x0 || unit == (void *)0x0) {
    return;
  }
  if (!BtlBakuganIsLocalPlayer(unit)) {
    return;
  }
  if (BtlBakuganGetTarget(unit) == (void *)0x0) {
    return;
  }
  obj = (ActorStageObj *)g_actorStageObjList->head;
  while (obj != (ActorStageObj *)0x0) {
    const VtblEntry *vt = (const VtblEntry *)obj->base.base.base.vtable;

    obj->base.targeted = 0;
    if (((int (*)(void *))vt[14].fn)((u8 *)obj + vt[14].delta) != 0) {
      void *mine = obj->unit;
      if (mine == BtlBakuganGetTarget(unit)) {
        obj->base.targeted = 1;
      }
    } else {
      vt = (const VtblEntry *)obj->base.base.base.vtable;
      if (((int (*)(void *))vt[15].fn)((u8 *)obj + vt[15].delta) != 0) {
        void *mine = obj->unit;
        if (mine == BtlBakuganGetTarget(unit)) {
          obj->base.targeted = 1;
        }
      }
    }
    obj = (ActorStageObj *)obj->base.base.base.next;
  }
}
