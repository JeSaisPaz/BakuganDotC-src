// bdc 0x088732d4 BtlBakuganState18Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 18 (`+0x140`), vtable slot `+0x160` called through
   `BtlBakuganRunState`. Short stun state: arms the collider (`hitTimer = 5`, flag bit 0), holds
   gravity and blocking, and counts `subTimer`. On frame 8 a player unit resets the global motion
   time scale to 1.0 and the main battle task's `flashTarget` to 0; after frame 8 it returns to
   state 0 and ends the guard effect unless the unit is still guarding with energy on the ground
   (guard command held, `BtlCombatHasEnergy`, not `BtlBakuganIsAirborne`). Every frame the
   horizontal velocity (x, z) is scaled by the time-step adjusted 0.7 retention. */

void BtlBakuganState18Update(BtlBakugan *self)
{
  CollisionCollider *collider;
  BtlMain *battle;
  float keep;

  collider = self->collider0;
  collider->hitTimer = 5;
  collider->flags |= 1;
  self->gravityHold = 1;
  self->blockTimer = 2;
  self->subTimer++;
  if (self->subTimer == 8) {
    if (self->isPlayer != 0) {
      GfxSetMotionTimeScale(1.0f);
      battle = (BtlMain *)BtlGetCameraTask();
      battle->flashTarget = 0.0f;
    }
  }
  else if (self->subTimer >= 9) {
    BtlBakuganSetState(self, 0, 0);
    if ((self->commands & 8) == 0 || BtlCombatHasEnergy(&self->combat) == 0 ||
        BtlBakuganIsAirborne(self, 1) != 0) {
      BtlBakuganEndGuardEffect(self);
    }
  }
  keep = BtlScaleRetentionByTimeStep(0.7f);
  self->base.velocity[0] = self->base.velocity[0] * keep;
  self->base.velocity[2] = self->base.velocity[2] * keep;
}
