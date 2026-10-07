// bdc 0x088733ec BtlBakuganState19Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 19, run through `BtlBakuganRunState`. Every frame it
   keeps the body collider marked hit (hitTimer 5, flags bit 0), holds gravity and damps the
   horizontal velocity by a retention of 0.96 (field `dashHeading` == 0) or 0.7 (otherwise),
   scaled to the time step. While motion 0x10d plays and its progress reaches `dashHeading` + 0.1,
   a player unit resets the motion time scale to 1 and the battle task's flash target to 0, then
   motion 0x10e starts (blend 0.2) and the motion speed is set to 1.2 (virtual slot 6). Otherwise,
   at progress 0.6 it returns to state 0 unless `BtlBakuganHandleIdleCommands` took a command;
   before that, at progress `dashHeading`, it only tries the idle commands. When it ran the idle
   commands (at 0.6 always, before only when one was taken) and the unit lacks the command bit 3,
   has no energy (`BtlCombatHasEnergy`) or is airborne, the guard effect ends
   (`BtlBakuganEndGuardEffect`). */

void BtlBakuganState19Update(BtlBakugan *self)
{
    CollisionCollider *body;
    float *velocity;
    float keep;
    const VtblEntry *setSpeed;
    bool endGuard;

    body = (CollisionCollider *)self->collider0;
    body->hitTimer = 5;
    body->flags |= 1;
    self->gravityHold = 1;
    velocity = self->base.velocity;
    if (self->dashHeading == 0.0f) {
        keep = BtlScaleRetentionByTimeStep(0.959999979f);
    } else {
        keep = BtlScaleRetentionByTimeStep(0.699999988f);
    }
    /* vscl.t of velocity.xyz, only x and z stored back (y kept) */
    velocity[0] = velocity[0] * keep;
    velocity[2] = velocity[2] * keep;

    if (BtlBakuganIsMotion(self, 0x10d) != 0) {
        if (!GfxModelMotionReached(&self->base, self->dashHeading + 0.100000001f)) {
            return;
        }
        if (self->isPlayer != 0) {
            GfxSetMotionTimeScale(1.0f);
            ((BtlMain *)BtlGetCameraTask())->flashTarget = 0.0f;
        }
        BtlBakuganPlayMotion(0.200000003f, self, 0x10e, 0, 0);
        setSpeed = &((const VtblEntry *)self->base.base.vtable)[6];
        ((float (*)(float, void *))setSpeed->fn)(1.20000005f, (u8 *)self + setSpeed->delta);
        return;
    }

    endGuard = false;
    if ((self->commands & 8) == 0 || BtlCombatHasEnergy(&self->combat) == 0 ||
        BtlBakuganIsAirborne(self, 1) != 0) {
        endGuard = true;
    }
    if (GfxModelMotionReached(&self->base, 0.600000024f)) {
        if (BtlBakuganHandleIdleCommands(self, 0) == 0) {
            BtlBakuganSetState(self, 0, 0);
        }
        if (endGuard) {
            BtlBakuganEndGuardEffect(self);
        }
    } else if (GfxModelMotionReached(&self->base, self->dashHeading) &&
               BtlBakuganHandleIdleCommands(self, 0) != 0 && endGuard) {
        BtlBakuganEndGuardEffect(self);
    }
}
