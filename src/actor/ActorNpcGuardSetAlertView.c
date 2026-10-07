// bdc 0x088e8c28 ActorNpcGuardSetAlertView
#include "bdc.h"

/* Vtable slot 46 of the guard classes: alert view (distance 85, half-angle 0.785) when `alert`,
   normal view (53, 0.314) otherwise. */

void ActorNpcGuardSetAlertView(ActorNpc *self, u8 alert)

{
  if (alert != '\0') {
    self->viewDist = 85.0f;
    self->viewHalfAngle = 0.7853982f;
    return;
  }
  self->viewDist = 53.0f;
  self->viewHalfAngle = 0.31415927f;
  return;
}

