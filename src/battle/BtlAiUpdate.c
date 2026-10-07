// bdc 0x088971e4 BtlAiUpdate
#include "bdc.h"

#define PI_F 3.14159274f
#define TWO_PI_F 6.28318548f

/* Calls virtual `slot` (a no-argument predicate) of `unit` through its GCC 2.x vtable entry. */
static int UnitVirtual(BtlBakugan *unit, int slot)
{
    const VtblEntry *entry = &((const VtblEntry *)unit->base.base.vtable)[slot];

    return ((int (*)(void *))entry->fn)((u8 *)unit + entry->delta);
}

/* Wraps a heading into (-pi, pi] with one +/-2pi step (a NaN takes the subtract branch). */
static float WrapHeading(float heading)
{
    if (!(heading <= PI_F)) {
        return heading - TWO_PI_F;
    }
    if (heading <= -PI_F) {
        return heading + TWO_PI_F;
    }
    return heading;
}

/* Copies a 16-byte vector (x, y, z, w). */
static void CopyQuad(float *dst, const float *src)
{
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}

/* Heading from the owner's position to `point` (x/z), wrapped to (-pi, pi]. */
static float HeadingToPoint(BtlBakugan *owner, const float *point)
{
    float dz = point[2] - owner->base.pos[2];

    return WrapHeading(atan2f(dz, point[0] - owner->base.pos[0]));
}

/* Per-frame update of the CPU AI object (0xa30 bytes, `BtlAiCreate`), called by the CPU unit
   update (`BtlCpuUnitUpdate`, `BtlUnitMode4Update`): rebuilds this frame's pad action bits
   from the previous frame's (bits 0x10010 kept, or only 0x206 of them while 0x20800 is held),
   re-validates the owner with `BtlBakuganListFind` (returns when it is gone or its input is
   disabled), snapshots the owner position into `lastPos` and the wander home on the first frame
   (start timer 2.0 s), re-validates the target (a dead / respawn-protected target is dropped with
   `BtlAiSetTarget`, the owner's AI actions cleared and the attack channel reset; a target
   answering virtual slot 10 with status 9 active is dropped). A dead or respawn-protected owner
   only gets its AI actions cleared. Otherwise, unless the owner is busy (state flags 0x30000000,
   state 4 or 5), a stick move latched by `BtlAiPadLatchStick` sets the owner's input heading:
   towards `detourPoint` while move flag 0x4000 is set, else for active layer 0/4 the pad's stick
   heading plus the owner's yaw (plus `BtlAiRelativeAngle` to the target when there is one), for
   layers 1..3 towards `goal`, each wrapped to (-pi, pi]. Then counts the frames the target sits in
   state 0, adds the distance moved since last frame to `odometer`, updates the target
   height (`BtlAiUpdateTargetHeight`), advances the start timer by 1/30 s (expiry resets the
   attack channel), stores the kind of the owner's last melee attacker in `pendingHit` (-1 none),
   clears move flags 0x8/0x10/0x40/0x400/0x800/0x8000/0x10000 and `rayHitsSoft`, counts down
   `chargeCooldown` and `forwardDashFrames` (floored at 0), refreshes target occlusion
   (`BtlAiUpdateTargetOcclusion`) every frame while `g_btlAiTargetLossHook` is set, else when
   the target changed or on the owner's frame slot (`g_btlSceneFrameCount & 3` equal to its list
   index & 3), runs the behaviour layers (`BtlAiRunLayers`, skipped while `guardRoll` is set and
   the owner's counter window is open; a layer change clears `guardRoll`/`dodgePressed`), stores
   the owner position as `lastPos`, saves the pad state (`BtlAiPadSavePrevious`) and hands this
   frame's pad action bits to the owner's input. */
void BtlAiUpdate(BtlAi *self)
{
    u32 prevActions = self->pad.prev.aiActions;
    BtlBakugan *owner;
    BtlBakugan *target;
    BtlBakugan *attacker;
    float moved;
    s32 hitKind;
    int runLayers;

    self->pad.cur.aiActions = 0;
    if ((prevActions & 0x10010) != 0) {
        if ((prevActions & 0x20800) != 0) {
            self->pad.cur.aiActions |= prevActions & 0x206;
        } else {
            self->pad.cur.aiActions |= prevActions & 0x10010;
        }
    }

    self->owner = BtlBakuganListFind(self->owner);
    if (self->owner == NULL) {
        return;
    }
    if (self->owner->input->disabled != 0) {
        return;
    }
    self->pad.owner = self->owner;
    if (self->firstFrame != 0) {
        CopyQuad(self->lastPos, self->owner->base.pos);
        CopyQuad(self->wander.home, self->owner->base.pos);
        self->startLimit = 2.0f;
        self->startElapsed = 0.0f;
        self->startExpired = 0;
        self->firstFrame = 0;
    }

    self->target = BtlBakuganListFind(self->target);
    if (self->target != NULL) {
        target = self->target;
        if (target->combat.dead != 0 || target->respawnProtect != 0) {
            BtlAiSetTarget(self, NULL);
            self->owner->input->aiActions = 0;
            BtlAiChannelReset(&self->channels[1]);
        } else if (UnitVirtual(self->target, 10) != 0 && self->target->combat.status[9].active != 0) {
            BtlAiSetTarget(self, NULL);
        }
    }

    owner = self->owner;
    if (owner->combat.dead != 0 || owner->respawnProtect != 0) {
        self->owner->input->aiActions = 0;
        return;
    }

    if ((self->owner->stateFlags & 0x30000000) == 0 && self->owner->state != 4 &&
        self->owner->state != 5 && BtlAiPadLatchStick(&self->pad) != 0) {
        if ((self->moveFlags & 0x4000) != 0) {
            self->owner->input->heading = HeadingToPoint(self->owner, self->detourPoint);
        } else if ((u32)self->activeLayer < 5) {
            if (self->activeLayer == 0 || self->activeLayer == 4) {
                float heading = self->pad.stickHeading;

                if (self->target == NULL) {
                    heading = heading + self->owner->base.rot[1];
                } else {
                    float relative = BtlAiRelativeAngle(self, self->owner, self->target);

                    heading = (self->owner->base.rot[1] + relative) + heading;
                }
                self->owner->input->heading = WrapHeading(heading);
            } else {
                self->owner->input->heading = HeadingToPoint(self->owner, self->goal);
            }
        }
    }

    if (self->target == NULL) {
        self->targetIdleFrames = 0.0f;
    } else if (self->target->state == 0) {
        self->targetIdleFrames = self->targetIdleFrames + 1.0f;
    } else {
        self->targetIdleFrames = 0.0f;
    }

    /* |lastPos - pos| over x/y/z. */
    {
        const float *pos = self->owner->base.pos;
        float dx = self->lastPos[0] - pos[0];
        float dy = self->lastPos[1] - pos[1];
        float dz = self->lastPos[2] - pos[2];

        moved = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    }
    self->odometer = self->odometer + moved;
    BtlAiUpdateTargetHeight(self);

    if (self->startExpired == 0) {
        self->startElapsed = self->startElapsed + 0.0333333351f;
        if (!(self->startElapsed < self->startLimit)) {
            self->startElapsed = self->startLimit;
            self->startExpired = 1;
        }
        if (self->startExpired != 0) {
            BtlAiChannelReset(&self->channels[1]);
        }
    }

    hitKind = -1;
    attacker = self->owner->meleeHitAttacker;
    if (attacker != NULL) {
        hitKind = (s32)attacker->base.base.unk08;
    }
    self->pendingHit = hitKind;
    self->moveFlags &= ~(0x8u | 0x10u | 0x400u | 0x800u | 0x8000u | 0x10000u | 0x40u);
    self->rayHitsSoft = 0;
    self->chargeCooldown = self->chargeCooldown - 1;
    if ((float)self->chargeCooldown <= 0.0f) {
        self->chargeCooldown = 0;
    }
    self->forwardDashFrames = self->forwardDashFrames - 1;
    if (self->forwardDashFrames <= 0) {
        self->forwardDashFrames = 0;
    }

    if (g_btlAiTargetLossHook != 0) {
        BtlAiUpdateTargetOcclusion(self);
    } else {
        int changed = self->targetChanged != 0;

        if (!changed) {
            u32 frameSlot = (u32)g_btlSceneFrameCount & 3;

            changed = frameSlot == ((u32)BtlBakuganListIndexOf(self->owner) & 3);
        }
        if (changed) {
            BtlAiUpdateTargetOcclusion(self);
            self->targetChanged = 0;
        }
    }

    runLayers = 1;
    if (self->guardRoll != 0) {
        runLayers = BtlBakuganIsCounterWindowOpen(self->owner) == 0;
    }
    if (runLayers && BtlAiRunLayers(self)) {
        self->guardRoll = 0;
        self->dodgePressed = 0;
    }

    CopyQuad(self->lastPos, self->owner->base.pos);
    BtlAiPadSavePrevious(&self->pad);
    self->owner->input->aiActions = 0;
    self->owner->input->aiActions |= self->pad.cur.aiActions;
}
