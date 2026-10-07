// bdc 0x08855344 ActorCrystalSetMode
#include "bdc.h"

/* Sets the crystal's behaviour mode `mode` (handler table `0x08a670ec`, see
   `ActorCrystalUpdateLogic`); unless `keepTimers`, clears `step` and `timer`; on entering mode
   1 (attack) clears `fireParam` and the vec4 `firePoint` (the VFPU bank zero vector C720). */

void ActorCrystalSetMode(ActorCrystal *self, int mode, bool keepTimers)
{
  self->mode = mode;
  if (!keepTimers) {
    self->step = 0;
    self->timer = 0;
  }
  if ((0 < self->mode) && (self->mode < 2)) {
    self->fireParam = 0.0f;
    self->firePoint[0] = 0.0f;
    self->firePoint[1] = 0.0f;
    self->firePoint[2] = 0.0f;
    self->firePoint[3] = 0.0f;
  }
}
