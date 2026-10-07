// bdc 0x08872fb8 BtlBakuganState14Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 14 (`state`), run through `BtlBakuganRunState`.
   Landing recovery: from 20% of the motion an attack/art command, a state-0xc command (bit 2) or a
   state-2 command (bit 4, both only with `BtlCombatHasEnergy`) leaves the state; from 40% the move
   command (bit 1) enters state 1, and at 90% it returns to state 0, both storing the horizontal
   speed |velocity.xz| in `dashSpeed`. Otherwise it applies hover, damps velocity x/z by 0.92 (motion
   0xc) or 0.8, and steps `subTimer`: 0 → landing effect plus a camera shake for a grounded local
   player; 1 → stage-object contact hook; afterwards skid dust while sliding (squared XZ speed > 4)
   for units with `moveStyle` set and no hover height. */
void BtlBakuganState14Update(BtlBakugan *self)
{
    float *vel = self->base.velocity;
    float speed;

    if (GfxModelMotionReached(&self->base, 0.200000003f)) {
        if (BtlBakuganHasAttackCommand(self)) {
            BtlBakuganStartAttackOrArt(self, 0);
            return;
        }
        if ((self->commands & 2) != 0 && BtlCombatHasEnergy(&self->combat)) {
            BtlBakuganSetState(self, 0xc, 0);
            return;
        }
        if ((self->commands & 4) != 0 && BtlCombatHasEnergy(&self->combat)) {
            BtlBakuganSetState(self, 2, 0);
            return;
        }
    }
    if (GfxModelMotionReached(&self->base, 0.400000006f) && (self->commands & 1) != 0) {
        /* |velocity| with y zeroed */
        speed = __builtin_sqrtf(vel[0] * vel[0] + 0.0f * 0.0f + vel[2] * vel[2]);
        self->dashSpeed = speed;
        BtlBakuganSetState(self, 1, 0);
        return;
    }
    if (GfxModelMotionReached(&self->base, 0.899999976f)) {
        speed = __builtin_sqrtf(vel[0] * vel[0] + 0.0f * 0.0f + vel[2] * vel[2]);
        self->dashSpeed = speed;
        BtlBakuganSetState(self, 0, 0);
        return;
    }
    BtlBakuganApplyHover(self);
    /* velocity.x and .z *= damping (0.92 in motion 0xc, else 0.8); y untouched */
    if (BtlBakuganIsMotion(self, 0xc)) {
        vel[0] = vel[0] * 0.920000017f;
        vel[2] = vel[2] * 0.920000017f;
    } else {
        vel[0] = vel[0] * 0.800000012f;
        vel[2] = vel[2] * 0.800000012f;
    }
    if (self->subTimer == 0) {
        self->subTimer = 1;
        BtlBakuganSpawnLandingEffect(self, 0);
        if (BtlBakuganIsLocalPlayer(self) && self->combat.stats->hoverHeight == 0.0f) {
            GfxCameraStartShake(9.0f, 0.5f, g_gfxActiveCamera, 0x14);
        }
        return;
    }
    if (self->subTimer == 1) {
        BtlBakuganNotifyContactStageObj(self);
        self->subTimer = 2;
    }
    if (self->combat.stats->moveStyle != 0 && self->combat.stats->hoverHeight == 0.0f) {
        /* squared speed with y zeroed */
        speed = vel[0] * vel[0] + 0.0f * 0.0f + vel[2] * vel[2];
        if (!(speed <= 4.0f)) {
            BtlBakuganSpawnSkidEffect(self);
        }
    }
}
