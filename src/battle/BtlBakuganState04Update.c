// bdc 0x08873b34 BtlBakuganState04Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 4 (`+0x140`), vtable slot `+0xf0` called through
   `BtlBakuganRunState` (and non-virtually by `BtlUnitAltState04Update`). Airborne / landing
   state: every frame sets `stateFlags` bits 0x4040000 and counts `stateCounter`, then runs
   sub-state `subTimer`:
   - -1: hover/slide: low motion id word into `stateFlags`, `subWait`++; unless hovering
     (`BtlBakuganApplyHover`) a footstep effect (foot 3, 2 on odd frames) when the speed squared
     is not <= 64; damps velocity x/z by 0.9 and lands (`BtlBakuganLand`, splash) once the
     horizontal speed squared is < 225.
   - 0: falling: arms the hit collider (flag 1, `hitTimer` 1) in the first 5 frames, `subWait`++,
     gravity 2.3 * 1.5 on `velocity.y` (2.3 * 0.75 below 10 when `flags` bit 2 is set), moves
     (`BtlBakuganApplyVelocity`) and counts `airborneFrames` while `stateFlags` bit 30 is set;
     kind sound 0xb on frame 30; tilts to the velocity (`BtlBakuganTiltToVelocity`, blend =
     height above `groundPoint` / 100 below 100 for kind 10, else 1) and eases `tiltStep` toward
     0.5; when the ground is reached within i = 1..4 frames (`BtlCalcDeceleratingDistance`)
     eases `orient` 1/(i+1) toward `g_vecUp`. A unit that may be knocked out
     (`BtlBakuganCanStartKnockOut`) sets focus point 30. While falling (`velocity.y < 0`) it
     counts `airRecoverDelay` down, goes to state 6 for a knock-out, to air recovery 0x17 (input
     charge/dodge cooldowns 5/10, commands 0x12 cleared, collider `hitTimer` 8) for a hit unit
     (`stateFlags` 0x20, alive) with commands 0x4012 once the delay is 0, and otherwise lands when
     `subWait` >= 151, the next height (`pos.y + velocity.y * 0.8`) is under `groundPoint.y` or
     `stateFlags` bit 31 is set.
   - 1: landing (collider `hitTimer` 16): `BtlBakuganLand` once at 35 % of the motion, at 70 %
     motion 0xf6 (0xf8 for `g_btlBakuganLandFlipMotionIds`) and next sub-state; x/z damped by
     0.95.
   - 2: get-up wait (collider `hitTimer` 16): state 6 on knock-out, 0x15 for a dead unit with
     `knockOutMode`; an alive unit whose motion ended or with commands 0x17 plays 0xf9 (0xfa
     flip) and advances, or with command 0x2000 too dashes: motion 0x1a (flip keeps 0xfa),
     sub-state 5, `dashHeading` from the input, velocity 32 along it and 17 up; x/z damped 0.85.
   - 3: energy regen; at 80 % of the motion state 0 (collider `hitTimer` 10); x/z damped 0.85.
   - 4: energy regen; once `stateFlags` bit 30 is clear: state 0 (collider `hitTimer` 10 after 13
     `subWait` frames), x/z damped 0.85.
   - 5: energy regen; when `velocity.y < 0` collider `hitTimer` 10, motion 0x1b, next sub-state.
   - 6: energy regen; once `stateFlags` bit 30 is clear: motion 0x1c, after 13 `subWait` frames
     state 0 (collider `hitTimer` 10); x/z damped 0.85.
   Then `effectAnchor` follows the "Bip01" node, and every 4th frame in sub-state 1 with a
   horizontal speed squared not <= 1 it plays kind sound 4 on water (`BtlBakuganIsOnWaterFloor`)
   or spawns dust effect 3 (8 in arenas 12..15) on `g_worldEffectMgr` at `groundPoint` + a
   random x/z offset in [-40, 40). */
void BtlBakuganState04Update(BtlBakugan *self)
{
    float vel[4];
    float focus[4];
    float buf[4];
    float off[4];
    ScePspFVector4 node;
    CollisionCollider *col;
    float blend;
    float dist;
    float keep;
    float speedSq;
    float vy;
    s32 frames;
    s32 i;
    s32 motion;
    s32 effectId;
    s32 wait;
    u8 anyAction;

    self->stateFlags |= 0x4040000;
    self->stateCounter = self->stateCounter + 1;

    switch (self->subTimer) {
    case -1:
        self->stateFlags |= self->motionIdPair[0];
        self->subWait = self->subWait + 1;
        if (BtlBakuganApplyHover(self) == 0) {
            speedSq = self->base.velocity[0] * self->base.velocity[0] +
                      self->base.velocity[1] * self->base.velocity[1] +
                      self->base.velocity[2] * self->base.velocity[2];
            if (!(speedSq <= 64.0f)) {
                BtlBakuganSpawnFootstepEffect(40.0f, 2.0f, self, (self->stateCounter & 1) != 0 ? 2 : 3);
            }
        }
        keep = BtlScaleRetentionByTimeStep(0.9f);
        self->base.velocity[0] = self->base.velocity[0] * keep;
        self->base.velocity[2] = self->base.velocity[2] * keep;
        speedSq = self->base.velocity[0] * self->base.velocity[0] +
                  self->base.velocity[2] * self->base.velocity[2];
        if (speedSq < 225.0f) {
            BtlBakuganLand(self, 0, 1);
        }
        break;

    case 0:
        if (self->stateCounter < 5) {
            col = self->collider0;
            col->hitTimer = 1;
            col->flags |= 1;
        }
        self->stateFlags |= self->motionIdPair[0];
        self->subWait = self->subWait + 1;
        self->base.velocity[1] =
            self->base.velocity[1] -
            (((self->flags & 2) != 0 && self->base.velocity[1] < 10.0f) ? 0.75f : 1.5f) * 2.3f;

        vel[0] = self->base.velocity[0];
        vel[1] = self->base.velocity[1];
        vel[2] = self->base.velocity[2];
        vel[3] = self->base.velocity[3];
        BtlBakuganApplyVelocity(self, vel);

        frames = 0;
        if ((self->stateFlags & 0x40000000) != 0) {
            frames = self->airborneFrames + 1;
        }
        self->airborneFrames = frames;
        if (self->stateCounter == 30) {
            BtlBakuganPlayKindSound(self, 0xb, 0, 0);
        }

        blend = 1.0f;
        if (self->base.base.unk08 == 10) {
            blend = self->base.pos[1] - self->groundPoint[1];
            if (blend < 100.0f) {
                blend = blend * 0.01f;
            } else {
                blend = 1.0f;
            }
        }
        BtlBakuganTiltToVelocity(self->tiltStep, blend, self);
        self->tiltStep = self->tiltStep + (0.5f - self->tiltStep) * 0.1f;

        /* the listing passes self in the callee's unused third argument */
        for (i = 1; i < 5; i++) {
            dist = BtlCalcDeceleratingDistance(self->base.velocity[1], self->gravity,
                                               (u32)(uintptr_t)self, i);
            if (dist + self->base.pos[1] < self->groundPoint[1]) {
                /* orient += (g_vecUp - orient) * (1 / (i + 1)) */
                keep = 1.0f / (float)(i + 1);
                self->orient[0] = self->orient[0] + (g_vecUp.x - self->orient[0]) * keep;
                self->orient[1] = self->orient[1] + (g_vecUp.y - self->orient[1]) * keep;
                self->orient[2] = self->orient[2] + (g_vecUp.z - self->orient[2]) * keep;
                self->orient[3] = self->orient[3] + (g_vecUp.w - self->orient[3]) * keep;
                break;
            }
        }

        if (BtlBakuganCanStartKnockOut(self) != 0) {
            focus[0] = 0.0f;
            focus[1] = 0.0f;
            focus[2] = 0.0f;
            focus[3] = 0.5f;
            BtlMainSetFocusPointGlobal(30, focus);
        }
        if (!(self->base.velocity[1] < 0.0f)) {
            break;
        }

        if (self->airRecoverDelay != 0) {
            self->airRecoverDelay = self->airRecoverDelay - 1;
        }
        if (BtlBakuganCanStartKnockOut(self) != 0) {
            BtlBakuganSetState(self, 6, 0);
            break;
        }
        if ((self->stateFlags & 0x20) != 0 && self->combat.dead == 0 &&
            (self->commands & 0x4012) != 0) {
            if (self->airRecoverDelay != 0) {
                break;
            }
            self->input->chargeCooldown = 5;
            self->input->dodgeCooldown = 10;
            self->commands &= ~0x12u;
            BtlBakuganSetState(self, 0x17, 0);
            col = self->collider0;
            col->hitTimer = 8;
            col->flags |= 1;
            break;
        }
        if (self->subWait >= 151 ||
            self->base.pos[1] + self->base.velocity[1] * 0.8f < self->groundPoint[1] ||
            (self->stateFlags & 0x80000000) == 0x80000000) {
            self->airRecoverDelay = 0;
            BtlBakuganLand(self, 0, 0);
        }
        break;

    case 1:
        col = self->collider0;
        col->hitTimer = 16;
        col->flags |= 1;
        self->stateFlags |= self->motionIdPair[0];
        if (GfxModelMotionReached(&self->base, 0.35f) && self->subWait == 0) {
            BtlBakuganLand(self, 0, 0);
            self->subWait = 1;
        }
        if (GfxModelMotionReached(&self->base, 0.7f)) {
            self->subWait = 1;
            motion = 0xf6;
            if (self->motionIdPair[0] == g_btlBakuganLandFlipMotionIds[0] &&
                self->motionIdPair[1] == g_btlBakuganLandFlipMotionIds[1]) {
                motion = 0xf8;
            }
            BtlBakuganPlayMotion(0.2f, self, motion, 0, 0);
            self->subTimer = self->subTimer + 1;
        }
        keep = BtlScaleRetentionByTimeStep(0.95f);
        self->base.velocity[0] = self->base.velocity[0] * keep;
        self->base.velocity[2] = self->base.velocity[2] * keep;
        break;

    case 2:
        col = self->collider0;
        col->hitTimer = 16;
        col->flags |= 1;
        self->stateFlags |= self->motionIdPair[0];
        anyAction = (self->commands & 0x17) != 0;
        if (BtlBakuganCanStartKnockOut(self) != 0) {
            BtlBakuganSetState(self, 6, 0);
            break;
        }
        if (self->knockOutMode != 0 && self->combat.dead != 0) {
            BtlBakuganSetState(self, 0x15, 0);
            break;
        }
        if (self->combat.dead == 0 && (self->base.motionEnded | anyAction) != 0) {
            motion = 0xf9;
            if (self->motionIdPair[0] == g_btlBakuganLandFlipMotionIds[0] &&
                self->motionIdPair[1] == g_btlBakuganLandFlipMotionIds[1]) {
                motion = 0xfa;
            }
            self->subTimer = self->subTimer + 1;
            if (anyAction && (self->commands & 0x2000) != 0) {
                self->dashHeading = self->input->heading;
                if (motion == 0xf9) {
                    motion = 0x1a;
                }
                self->subTimer = 5;
                /* velocity = {cos, 0, sin, 0}(dashHeading) with x/y/z scaled by 32 */
                self->base.velocity[0] = __builtin_cosf(self->dashHeading) * 32.0f;
                self->base.velocity[1] = 0.0f;
                self->base.velocity[2] = __builtin_sinf(self->dashHeading) * 32.0f;
                self->base.velocity[3] = 0.0f;
                self->base.velocity[1] = 17.0f;
                self->subWait = 0;
            }
            BtlBakuganPlayMotion(0.0f, self, motion, 0, 0);
        }
        keep = BtlScaleRetentionByTimeStep(0.85f);
        self->base.velocity[0] = self->base.velocity[0] * keep;
        self->base.velocity[2] = self->base.velocity[2] * keep;
        break;

    case 3:
        self->stateFlags |= self->motionIdPair[0];
        BtlCombatTickEnergyRegen(&self->combat, 2);
        if (GfxModelMotionReached(&self->base, 0.8f)) {
            col = self->collider0;
            col->hitTimer = 10;
            col->flags |= 1;
            BtlBakuganSetState(self, 0, 0);
        }
        keep = BtlScaleRetentionByTimeStep(0.85f);
        self->base.velocity[0] = self->base.velocity[0] * keep;
        self->base.velocity[2] = self->base.velocity[2] * keep;
        break;

    case 4:
        self->stateFlags |= self->motionIdPair[0];
        BtlCombatTickEnergyRegen(&self->combat, 2);
        if ((self->stateFlags & 0x40000000) != 0) {
            break;
        }
        wait = self->subWait;
        self->subWait = wait + 1;
        if (wait >= 13) {
            col = self->collider0;
            col->hitTimer = 10;
            col->flags |= 1;
        }
        BtlBakuganSetState(self, 0, 0);
        keep = BtlScaleRetentionByTimeStep(0.85f);
        self->base.velocity[0] = self->base.velocity[0] * keep;
        self->base.velocity[2] = self->base.velocity[2] * keep;
        break;

    case 5:
        self->stateFlags |= self->motionIdPair[0];
        BtlCombatTickEnergyRegen(&self->combat, 2);
        if (self->base.velocity[1] < 0.0f) {
            col = self->collider0;
            col->hitTimer = 10;
            col->flags |= 1;
            BtlBakuganPlayMotion(0.2f, self, 0x1b, 0, 0);
            self->subTimer = self->subTimer + 1;
        }
        break;

    case 6:
        self->stateFlags |= self->motionIdPair[0];
        BtlCombatTickEnergyRegen(&self->combat, 2);
        if ((self->stateFlags & 0x40000000) != 0) {
            break;
        }
        BtlBakuganPlayMotion(0.2f, self, 0x1c, 0, 0);
        wait = self->subWait;
        self->subWait = wait + 1;
        if (wait >= 13) {
            col = self->collider0;
            col->hitTimer = 10;
            col->flags |= 1;
            BtlBakuganSetState(self, 0, 0);
        }
        keep = BtlScaleRetentionByTimeStep(0.85f);
        self->base.velocity[0] = self->base.velocity[0] * keep;
        self->base.velocity[2] = self->base.velocity[2] * keep;
        break;

    default:
        break;
    }

    GfxModelGetNodeWorldPos(&self->base, &node, "Bip01");
    self->effectAnchor[0] = node.x;
    self->effectAnchor[1] = node.y;
    self->effectAnchor[2] = node.z;
    self->effectAnchor[3] = node.w;

    if ((self->stateCounter & 3) != 0 || self->subTimer != 1) {
        return;
    }
    speedSq = self->base.velocity[0] * self->base.velocity[0] +
              self->base.velocity[2] * self->base.velocity[2];
    if (speedSq <= 1.0f) {
        return;
    }
    if (BtlBakuganIsOnWaterFloor(self) != 0) {
        BtlBakuganPlayKindSound(self, 4, 0, 0);
        return;
    }
    buf[0] = 80.0f;
    buf[1] = 0.0f;
    buf[2] = 80.0f;
    buf[3] = 0.0f;
    /* off.xyz = buf.xyz * random[0, 1) (vrndf1 gives [1, 2), minus 1); the asm's w lane is an
       unused stale register, dropped */
    off[0] = PlatformRandFloat12();
    off[1] = PlatformRandFloat12();
    off[2] = PlatformRandFloat12();
    off[0] = buf[0] * (off[0] - 1.0f);
    off[1] = buf[1] * (off[1] - 1.0f);
    off[2] = buf[2] * (off[2] - 1.0f);
    off[3] = 0.0f;
    buf[0] = 40.0f;
    buf[1] = 0.0f;
    buf[2] = 40.0f;
    buf[3] = 0.0f;
    off[0] = off[0] - buf[0];
    off[1] = off[1] - buf[1];
    off[2] = off[2] - buf[2];
    effectId = 3;
    if (g_btlArenaIndex >= 12 && g_btlArenaIndex < 16) {
        effectId = 8;
    }
    buf[0] = self->groundPoint[0] + off[0];
    buf[1] = self->groundPoint[1] + off[1];
    buf[2] = self->groundPoint[2] + off[2];
    buf[3] = self->groundPoint[3];
    GfxEffectSpawn(g_worldEffectMgr, effectId, buf);
}
