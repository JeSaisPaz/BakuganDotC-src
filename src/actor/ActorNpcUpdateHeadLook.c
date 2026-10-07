// bdc 0x088e6214 ActorNpcUpdateHeadLook
#include "bdc.h"

/* Vtable slot 47 of three guard classes: runs `ActorNpcViewConeUpdate` on the view-cone object
   `+0x418` with mode 2, weight 1, the view distance `+0x408`, `angle` and the view heading
   `+0x420`. */

void ActorNpcUpdateHeadLook(float angle, ActorNpc *self)

{
  ActorNpcViewConeUpdate(self->viewCone,2,self->viewDist,angle,self->viewHeading,1.0f);
  return;
}

