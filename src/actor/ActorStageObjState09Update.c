// bdc 0x088aa134 ActorStageObjState09Update
#include "bdc.h"

/* State 9 handler of the shared stage-object state machine (table `0x08a842f8`,
   `ActorStageObjUpdate`): when virtual `+0x64` reports activation, runs
   `ActorStageObjAttrLandmarkSetActivated`; then makes the collider non-blocking (flag 2), waits
   250 frames (`+0x308`) and marks the object for removal (`+0x282`). */

void ActorStageObjState09Update(ActorStageObjBase *self)
{
  int step = self->step;

  if (step < 1) {
    const MemberFnPtr *fn;

    if (step < 0) {
      return;
    }
    fn = (const MemberFnPtr *)self->base.base.vtable + 12; /* vtable +0x60 */
    if (((int (*)(void *))fn->pfn)((char *)self + fn->delta) != 0) {
      ActorStageObjAttrLandmarkSetActivated((ActorStageObjAttrLandmark *)self);
    }
    self->timer = 0;
    self->step = self->step + 1;
  } else if (step >= 2) {
    if (step >= 3) {
      return;
    }
    if (self->timer != 0) {
      self->timer = self->timer - 1;
      return;
    }
    self->removeRequest = 1;
    return;
  }
  ((CollisionCollider *)self->collider)->flags |= 2;
  self->timer = 0xfa;
  self->step = self->step + 1;
}
