// bdc 0x08864abc BtlBakuganApplyGravity
#include "bdc.h"

/* Per-frame gravity and velocity integration of a battle unit (from `BtlBakuganUpdate`): flag
   0x400000 forces the suspension timer `gravityHold` to 4; while it runs, `gravity` decays by 20 %
   per frame and the timer counts down, otherwise `gravity` eases 20 % per frame towards the stat
   table's `gravity` (`gravityAlt` while flag 0x1000000 is set). Unless movement is frozen (flag 2)
   it subtracts `gravity × motion time scale` from the vertical velocity, moves the unit by the
   velocity xyz scaled by the time scale (`BtlBakuganApplyVelocity`), and either counts
   `airborneFrames` (flag 0x40000000) or zeroes the vertical velocity and that counter. */

void BtlBakuganApplyGravity(BtlBakugan *self)
{
    float step[4];
    float target;
    float velY;
    float scale;

    if ((self->stateFlags & 0x400000) != 0) {
        self->gravityHold = 4;
    }
    if (self->gravityHold != 0) {
        self->gravity = self->gravity * 0.800000012f;
        self->gravityHold = self->gravityHold - 1;
    } else {
        if ((self->stateFlags & 0x1000000) != 0) {
            target = self->combat.stats->gravityAlt;
        } else {
            target = self->combat.stats->gravity;
        }
        self->gravity = self->gravity + (target - self->gravity) * 0.200000003f;
    }
    if ((self->stateFlags & 2) != 0) {
        return;
    }
    velY = self->base.velocity[1];
    self->base.velocity[1] = velY - self->gravity * GfxGetMotionTimeScale();
    scale = GfxGetMotionTimeScale();
    /* step.xyz = velocity.xyz * scale (vscl.t); the sv.q also stores a lane 3 the function never
       set (stale C710.w), left out. */
    step[0] = self->base.velocity[0] * scale;
    step[1] = self->base.velocity[1] * scale;
    step[2] = self->base.velocity[2] * scale;
    BtlBakuganApplyVelocity(self, step);
    if ((self->stateFlags & 0x40000000) != 0) {
        self->airborneFrames = self->airborneFrames + 1;
    } else {
        self->base.velocity[1] = 0.0f;
        self->airborneFrames = 0;
    }
}
