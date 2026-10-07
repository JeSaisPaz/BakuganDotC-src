// bdc 0x08873618 BtlBakuganState00Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 0 (`+0x140`, idle / guard), vtable slot `+0xd0` called
   through `BtlBakuganRunState`. Guarding needs command bit 8 or a running `blockTimer`, energy
   (`BtlCombatHasEnergy`), `stateFrames` != 0 and not being airborne (`BtlBakuganIsAirborne`):
   then it sets state flag 0x200000, applies the energy of action 7 (`BtlBakuganApplyStateEnergy`),
   sets `regenDelay` to 15 and, while `subTimer` is 0, plays guard-start motion 0x101 (motion speed
   1.5, guard effect on) and at 80% of it the guard loop 0x103 (speed 1.0), or 0x103 directly with the
   guard effect when the kind lacks 0x101; `subTimer` becomes 1 once 0x103 runs. Otherwise `subTimer`
   is reset and from 0x103 it plays guard-end motion 0x102 (if defined; speed 1.0, guard effect off),
   from 0x102 at 80% the idle motion 0, else ends the guard effect. Unless the guard command is held
   (with `subTimer` != 0, and not at `subTimer` 1 with the model's motion blend at 0)
   `BtlBakuganHandleIdleCommands` runs; when it leaves states 0/1 the guard effect ends, and when it
   took no command and no guard motion runs, idle motion 0 is restarted. Then hover
   (`BtlBakuganApplyHover`); while guarding, velocity x/z are damped by 0.85, the unit turns toward
   its target (`BtlBakuganGetTarget`, `atan2f`, rate horizontal speed * 0.01, step at most 0.8) and
   above horizontal speed 8 spawns footstep dust (foot by `stateCounter` parity); otherwise x/z are
   damped by 0.7. Always counts `stateFrames` up. */
void BtlBakuganState00Update(BtlBakugan *self)
{
    float *vel = self->base.velocity;
    const VtblEntry *entry;
    BtlBakugan *target;
    float speed;
    float angle;
    s32 t;
    bool holdGuard = false;   /* guard command held or a guard transition motion running */
    bool skipCommands = true; /* with holdGuard: skip the idle command check */
    bool playIdle = false;    /* restart idle motion 0 */

    if ((self->commands & 8) == 0 && self->blockTimer == 0) {
        self->subTimer = 0;
        goto notGuarding;
    }
    if (!BtlCombatHasEnergy(&self->combat)) {
        self->subTimer = 0;
        goto notGuarding;
    }
    if (self->stateFrames == 0) {
        self->subTimer = 0;
        goto notGuarding;
    }
    if (BtlBakuganIsAirborne(self, 1)) {
        self->subTimer = 0;
        goto notGuarding;
    }

    if ((self->commands & 8) != 0) {
        holdGuard = true;
    }
    self->stateFlags |= 0x200000;
    BtlBakuganApplyStateEnergy(self, 7, 0);
    t = self->subTimer;
    self->combat.regenDelay = 15.0f;
    if (t > 0) {
        if (t < 2 && self->base.data->motionBlend == 0.0f) {
            skipCommands = false;
        }
    } else if (t == 0) {
        if (BtlBakuganIsMotion(self, 0x101)) {
            if (GfxModelMotionReached(&self->base, 0.800000012f)) {
                BtlBakuganPlayMotion(0.200000003f, self, 0x103, 1, 0);
                entry = &((const VtblEntry *)self->base.base.vtable)[6];
                ((float (*)(float, void *))entry->fn)(1.0f, (u8 *)self + entry->delta);
                self->subTimer = 1;
            }
        } else if (!BtlBakuganHasMotion(self, 0x101)) {
            BtlBakuganPlayMotion(0.200000003f, self, 0x103, 1, 0);
            self->subTimer = 1;
            BtlBakuganStartGuardEffect(self);
        } else if (BtlBakuganPlayMotion(0.200000003f, self, 0x101, 0, 0)) {
            entry = &((const VtblEntry *)self->base.base.vtable)[6];
            ((float (*)(float, void *))entry->fn)(1.5f, (u8 *)self + entry->delta);
            BtlBakuganStartGuardEffect(self);
        }
        skipCommands = false;
    }
    goto commands;

notGuarding:
    if (BtlBakuganIsMotion(self, 0x103) && BtlBakuganHasMotion(self, 0x102)) {
        BtlBakuganPlayMotion(0.200000003f, self, 0x102, 0, 0);
        entry = &((const VtblEntry *)self->base.base.vtable)[6];
        ((float (*)(float, void *))entry->fn)(1.0f, (u8 *)self + entry->delta);
        BtlBakuganEndGuardEffect(self);
        holdGuard = true;
    } else if (BtlBakuganIsMotion(self, 0x102)) {
        if (GfxModelMotionReached(&self->base, 0.800000012f)) {
            BtlBakuganPlayMotion(0.200000003f, self, 0, 1, 0);
            entry = &((const VtblEntry *)self->base.base.vtable)[6];
            ((float (*)(float, void *))entry->fn)(1.0f, (u8 *)self + entry->delta);
        }
        holdGuard = true;
    } else {
        playIdle = true;
        BtlBakuganEndGuardEffect(self);
    }

commands:
    if (!holdGuard || !skipCommands) {
        playIdle = BtlBakuganHandleIdleCommands(self, 0) == 0;
        if (self->state != 1 && self->state != 0) {
            BtlBakuganEndGuardEffect(self);
        }
    }
    if (playIdle && !holdGuard) {
        BtlBakuganPlayMotion(0.200000003f, self, 0, 1, 0);
        entry = &((const VtblEntry *)self->base.base.vtable)[6];
        ((float (*)(float, void *))entry->fn)(1.0f, (u8 *)self + entry->delta);
    }
    BtlBakuganApplyHover(self);

    if ((self->stateFlags & 0x200000) != 0) {
        /* velocity.x and .z *= 0.85; y untouched */
        vel[0] = vel[0] * 0.850000024f;
        vel[2] = vel[2] * 0.850000024f;
        target = BtlBakuganGetTarget(self);
        /* horizontal speed: length of velocity with y zeroed */
        speed = __builtin_sqrtf(vel[0] * vel[0] + 0.0f * 0.0f + vel[2] * vel[2]);
        if (target != NULL) {
            angle = atan2f(target->base.pos[2] - self->base.pos[2],
                           target->base.pos[0] - self->base.pos[0]);
            BtlBakuganTurnToward(angle, speed * 0.00999999978f, 0.800000012f, self);
        }
        self->stateCounter++;
        if (!(speed <= 8.0f)) {
            BtlBakuganSpawnFootstepEffect(40.0f, 2.0f, self, (self->stateCounter & 1) != 0 ? 2 : 3);
        }
    } else {
        /* velocity.x and .z *= 0.7; y untouched */
        vel[0] = vel[0] * 0.699999988f;
        vel[2] = vel[2] * 0.699999988f;
    }
    self->stateFrames++;
}
