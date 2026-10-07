// bdc 0x088e8ddc ActorNpcGuardUpdateHeadLook
#include "bdc.h"

/* Vtable slot 47 of the guard classes: `ActorNpcViewConeUpdate` on the view cone `+0x418` with
   the look mode `+0x450`, weight 1.273, the view distance and heading; then resets the mode to 2.
    */

void ActorNpcGuardUpdateHeadLook(float angle, ActorNpcGuard *self)

{
  ActorNpcViewConeUpdate
            ((self->base).viewCone,self->lookMode,(self->base).viewDist,angle,
             (self->base).viewHeading,(self->base).viewHalfAngle * 1.2732395f);
  if (self->lookMode != 2) {
    self->lookMode = 2;
  }
  return;
}

