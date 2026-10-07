// bdc 0x088a2740 ActorStageObjDeactivate
#include "bdc.h"

/* Switches a stage-object subclass "off": stops its looping effect (`ActorStageObjStopEffect`),
   destroys the collider at `obj + 0x32c` through its virtual destructor with flags 3 and clears the
   pointer, and clears `+0x680` of the Bakugan bound at `obj + 0x320`. Counterpart of
   `ActorStageObjActivate`. */

void ActorStageObjDeactivate(ActorStageObjLandmark *obj)
{
  ActorStageObjStopEffect((ActorStageObjCrystal *)obj);
  if (obj->helper != (CollisionCollider *)0) {
    const VtblEntry *dtor = &((const VtblEntry *)obj->helper->node.vtable)[1];
    ((void (*)(void *, s32))dtor->fn)((u8 *)obj->helper + dtor->delta, 3);
    obj->helper = (CollisionCollider *)0;
  }
  obj->unit->untargetable = 0;
}
