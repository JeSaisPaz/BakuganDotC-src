// bdc 0x088706b0 BtlBakuganSetState
#include "bdc.h"

/* State-machine setter of the battle Bakugan: stores `state` (the previous state is kept). When
   `keep` is 0 it first resets the per-state fields (subTimer, subWait, stateCounter, stateTicks,
   dashHeading/Lift/Bank = 0, effectAnchor and statePos = VFPU C720, footstep = 0, trailEffect =
   NULL), sets the motion speed to 1.0 (virtual slot 6), clears stateFlags bit 0x80000 and empties
   comboInputs (`g_btlComboInputsEmpty`). Then it stops kind sound 0 (if playing) and 4, for kind
   0x19 also sound 0x1c00000, ends the guard effect unless the new state is 0, clears attackResult
   and stateFrames and runs the entry action of the state:
   0 dashSpeed = 0; 1 motion 1; 2 dash: heading from the input (command bit 0) or the model yaw,
   motion 5, dashSpeed = scaled stat 0x48 (reduced toward stat 0x4c when the target lies within
   pi/10 of the heading), kind sound 0, airborneFrames = 4, effect 0x1c at bone "Bip01" along the
   heading; 7 attack: with status 0x13 active re-enters state 3 and plays motion 0xea, else
   restores the previous state when there are no attack motions or an art cancel (artCancelTimer
   with command 0x20000) is pending, else `BtlBakuganStartAttack` with the previous state;
   8/10 state energy 9 (charge == 100) or 8, artCancelTimer = 10; 0xb clears stateFlags 0x2000;
   0xc (unless motion 0x1b plays) state energy 1, motion 0x19, stats counter 0xc += 1, clears flags
   0x2000; 0xe landing motion 0x1c and the floor step sound (8, 9 on material 3, none on water
   outside stages 0xc..0xf); 0xf stops status effects and all loop sounds, velocity = C720;
   0x11 camera flash 0, talk start, motion time scale 1.0; 0x12/0x13 guard motions 0x103/0x10d
   (0x13 from 30 % of the motion) with the guard effect, slow motion / camera flash 0.7 for the
   player, collider hitTimer = 5 and flag 1, motionDone = 0 (0x13 also focus point 0x1e and
   dashHeading = 0.6); 0x17 motion 0x1b, kind sound 4, state becomes 0xc; 0x18 motion 0xc (blend
   0.05 for move style 2), state becomes 0xe. Other states only store the state.
   The reset vectors and velocity are zeroed from the VFPU bank's zero vector (C720). */
void BtlBakuganSetState(BtlBakugan *self, s32 state, char keep)
{
    float dir[4];
    float targetPos[4];
    ScePspFVector4 bonePos;
    float focus[4];
    const VtblEntry *vt;
    s32 prevState;
    float yaw;
    float diff;
    float turn;
    float margin;
    float speed;
    float stat48;
    float blend;
    s32 sound;
    s32 frame;
    GfxModel *target;
    GfxEffectMgr *mgr;
    BtlMain *cam;
    CollisionCollider *collider;

    prevState = self->state;
    self->state = state;
    if (keep == 0) {
        self->subTimer = 0;
        self->subWait = 0;
        self->stateCounter = 0;
        self->stateTicks = 0;
        self->dashHeading = 0.0f;
        self->dashLift = 0.0f;
        self->dashBank = 0.0f;
        self->effectAnchor[0] = 0.0f;
        self->effectAnchor[1] = 0.0f;
        self->effectAnchor[2] = 0.0f;
        self->effectAnchor[3] = 0.0f;
        self->statePos[0] = 0.0f;
        self->statePos[1] = 0.0f;
        self->statePos[2] = 0.0f;
        self->statePos[3] = 0.0f;
        self->footstep = 0;
        self->trailEffect = NULL;
        vt = &((const VtblEntry *)self->base.base.vtable)[6];
        ((float (*)(float, void *))vt->fn)(1.0f, (u8 *)self + vt->delta);
        self->stateFlags &= 0xfff7ffff;
        __builtin_memcpy(self->comboInputs, g_btlComboInputsEmpty, sizeof(self->comboInputs));
    }

    if (BtlBakuganHasKindSound(self, 0) != 0) {
        BtlBakuganStopKindSound(self, 0);
    }
    BtlBakuganStopKindSound(self, 4);
    if (self->base.base.unk08 == 0x19) {
        BtlBakuganStopSound(self, 0x1c00000);
    }
    if (self->state != 0) {
        BtlBakuganEndGuardEffect(self);
    }
    self->attackResult = 0;
    self->stateFrames = 0;

    switch (self->state) {
    case 0:
        self->dashSpeed = 0.0f;
        break;
    case 1:
        BtlBakuganPlayMotion(0.2f, self, 1, 1, 0);
        break;
    case 2:
        if ((self->commands & 1) != 0) {
            self->dashHeading = self->input->heading;
        } else {
            self->dashHeading = self->base.rot[1];
        }
        if (BtlBakuganGetTarget(self) != NULL) {
            yaw = self->dashHeading;
        } else {
            yaw = self->base.rot[1];
        }
        /* dir = (cos, 0, sin, 0) of yaw */
        dir[0] = __builtin_cosf(yaw);
        dir[1] = 0.0f;
        dir[2] = __builtin_sinf(yaw);
        dir[3] = 0.0f;
        BtlBakuganPlayMotion(0.2f, self, 5, 1, 0);
        self->dashSpeed = BtlBakuganGetScaledStat48(self);
        if (BtlBakuganGetTarget(self) != NULL) {
            target = (GfxModel *)BtlBakuganGetTarget(self);
            targetPos[0] = target->pos[0];
            targetPos[1] = target->pos[1];
            targetPos[2] = target->pos[2];
            targetPos[3] = target->pos[3];
            diff = self->dashHeading - atan2f(targetPos[2] - self->base.pos[2],
                                              targetPos[0] - self->base.pos[0]);
            diff = diff - (float)(s32)(diff * 0.318309873f) * 6.28318548f;
            if (diff < 0.0f) {
                diff += 6.28318548f;
            }
            if (diff < 3.14159274f) {
                turn = -diff;
            } else {
                turn = 6.28318548f - diff;
            }
            margin = 0.314159274f - __builtin_fabsf(turn);
            if (!(margin <= 0.0f)) {
                speed = self->dashSpeed;
                stat48 = BtlBakuganGetScaledStat48(self);
                self->dashSpeed =
                    speed - (stat48 - BtlBakuganGetScaledStat4C(self)) * margin * 3.18309879f;
            }
        }
        BtlBakuganPlayKindSound(self, 0, 1, 0);
        self->airborneFrames = 4;
        mgr = g_worldEffectMgr;
        GfxModelGetNodeWorldPos(&self->base, &bonePos, "Bip01");
        GfxEffectSpawnDirected(mgr, 0x1c, &bonePos.x, dir);
        break;
    case 7:
        if (self->combat.status[0x13].active != 0) {
            BtlBakuganSetState(self, 3, 0);
            BtlBakuganPlayMotion(0.0f, self, 0xea, 0, 1);
        } else if (self->attackMotions == NULL ||
                   (self->artCancelTimer != 0 && (self->commands & 0x20000) != 0)) {
            self->state = prevState;
        } else {
            BtlBakuganStartAttack(self, prevState);
        }
        break;
    case 8:
    case 10:
        if ((s32)self->charge == 100) {
            BtlBakuganApplyStateEnergy(self, 9, 0);
        } else {
            BtlBakuganApplyStateEnergy(self, 8, 0);
        }
        self->artCancelTimer = 10;
        break;
    case 0xb:
        self->stateFlags &= ~0x2000u;
        break;
    case 0xc:
        if (BtlBakuganIsMotion(self, 0x1b) != 0) {
            break;
        }
        BtlBakuganApplyStateEnergy(self, 1, 0);
        BtlBakuganPlayMotion(0.1f, self, 0x19, 0, 0);
        if (self->stats != NULL) {
            BtlStatsAddCounter(self->stats, 0xc, 1);
        }
        self->flags &= ~0x2000u;
        break;
    case 0xe:
        blend = 0.05f;
        if (self->base.base.unk08 == 8) {
            blend = 0.1f;
        }
        BtlBakuganPlayMotion(blend, self, 0x1c, 0, 0);
        sound = 8;
        if (self->floorMaterial < 4) {
            if (self->floorMaterial >= 3) {
                sound = 9;
            }
        } else if (self->floorMaterial < 5 && BtlBakuganIsInWaterStage0CTo0F(self) == 0) {
            sound = 0x19;
        }
        if (sound != 0x19) {
            BtlBakuganPlayKindSound(self, sound, 0, 0);
        }
        break;
    case 0xf:
        BtlBakuganStopStatusEffects(self, 0);
        BtlBakuganStopLoopSounds(self, true);
        self->base.velocity[0] = 0.0f;
        self->base.velocity[1] = 0.0f;
        self->base.velocity[2] = 0.0f;
        self->base.velocity[3] = 0.0f;
        break;
    case 0x11:
        cam = (BtlMain *)BtlGetCameraTask();
        cam->flashTarget = 0.0f;
        BtlMainStartTalk((BtlMain *)BtlGetCameraTask(), self);
        GfxSetMotionTimeScale(1.0f);
        break;
    case 0x12:
        BtlBakuganPlayMotion(0.0f, self, 0x103, 1, 0);
        BtlBakuganStartGuardEffect(self);
        if (self->isPlayer != 0) {
            GfxSetMotionTimeScale(0.75f);
            cam = (BtlMain *)BtlGetCameraTask();
            cam->flashTarget = 0.7f;
        }
        collider = self->collider0;
        collider->hitTimer = 5;
        collider->flags |= 1;
        self->motionDone = 0;
        break;
    case 0x13:
        BtlBakuganPlayMotion(0.0f, self, 0x10d, 0, 0);
        frame = GfxModelMotionEndScaled(&self->base, 0.3f);
        GfxModelSwapMotionFrame(&self->base, (float)frame);
        BtlBakuganStartGuardEffect(self);
        if (self->isPlayer != 0) {
            cam = (BtlMain *)BtlGetCameraTask();
            cam->flashTarget = 0.7f;
            focus[0] = 0.0f;
            focus[1] = 0.0f;
            focus[2] = 0.0f;
            focus[3] = 0.6f;
            BtlMainSetFocusPointGlobal(0x1e, focus);
        }
        collider = self->collider0;
        collider->hitTimer = 5;
        collider->flags |= 1;
        self->motionDone = 0;
        self->dashHeading = 0.6f;
        break;
    case 0x17:
        BtlBakuganPlayMotion(0.2f, self, 0x1b, 1, 0);
        BtlBakuganPlayKindSound(self, 4, 0, 0);
        self->state = 0xc;
        self->flags &= ~0x2000u;
        break;
    case 0x18:
        blend = 0.2f;
        if (self->combat.stats->moveStyle == 2) {
            blend = 0.05f;
        }
        BtlBakuganPlayMotion(blend, self, 0xc, 0, 0);
        self->state = 0xe;
        break;
    default:
        /* 3..6, 9, 0xd, 0x10, 0x14..0x16 and out of range: state only */
        break;
    }
}
