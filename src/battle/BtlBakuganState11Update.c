// bdc 0x0886df38 BtlBakuganState11Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 11 (`state`), vtable slot `+0x128` called through
   `BtlBakuganRunState`: the special-art attack (entered from `BtlBakuganState17Update` for art
   kind 2). Turns toward the target, sets state flag 0x400000 and runs `attackPhase`:
   - 0: flag 0x8000000 when the attack motion set (`BtlAttackMotionSet`) has flag 2; at 98 % of
     the motion (70 % with set flag 0x40) plays phase 1 (`BtlBakuganPlayAttackMotion`).
   - 1: once the opening hit landed (state flag 0x2000) on an existing target: arms `collider0`
     (`hitTimer` 10, flag 1), stores the target in `grabTarget` and either goes to state 0x14
     (set flag 0x20000: `hitTimer` 999, clears flag 0x2000, returns at once) or starts phase 2 at
     step 0 of the kind's `g_btlKindArtSequences` (`BtlArtSeqStep`: motion blend 0.067, speed
     through model vtable slot 6). Without a hit it disarms the collider and flashes in the
     attribute colour until `subWait` passes 19, then plays phase 3. Both set flag 0x1000000 and,
     for the local player, spawn effect 0x1d twice along the normalised velocity.
   - 2: keeps the collider armed, starts the cut-in once (`BtlMainStartCutIn`, unit flag 0x400),
     sets `comboTimer` 40, arms `g_btlArtHitWindows` per step flags (8: [0] at 48 %; at 95 %
     unless flag 0x10: [0] for no 0x7 bits, else [2]/[3]/[1] for 2/4/other and `stateCounter` 1),
     then at 95 % either ends (flag 0x20: state 0, model vtable slot 26, cut-in ended) or plays
     the next step (with 0x7 bits and `BtlBakuganTargetInvolvesPlayer`: time scale 0.5, camera
     `flashTarget` 0.8). Once a hit window was armed `stateTicks` counts up; at 0x15 the cut-in
     ends and slow motion is reset. Under 3 frames left on a 0x7 step: time scale 0.65.
   - 3: at 80 % back to state 0 (as above, plus time scale 1 / `flashTarget` 0) and releases the
     target's `lockedAttacker` when it is this unit.
   In phases 1..2 it sets flag 1 and points `orient` like `BtlBakuganState09Update`; phases < 2
   call `BtlBakuganSetStateFlag04`, phase 0 sets flag 0x2000000. Movement: in phase 0 with set
   flag 1 (and no 0x20) the vertical speed pulls to 110 above `groundY`; in phases 1..2 (or with
   set flag 0x20) before any hit window and without set flag 0x80 it dashes toward the target
   (speed stat 48 on the first frame, then clamped to the gap minus 1.2 x reach, easing to stat 4C)
   or forward at the melee step speed, lerping the velocity by 0.3. Without a target at entry
   velocity x/z are damped by 0.85. Counts `attackFrame`, and `attackEventFrame` from phase 2.
   Normalisations follow the listing: a zero-length vector gives 0 (bank S713), xyz clamped to
   [-1, 1], lane w 0. */

typedef void (*BtlSetMotionSpeedFn)(float speed, void *self);

void BtlBakuganState11Update(BtlBakugan *self)
{
    float dir[4];
    float norm[4];
    float delta[4];
    float step[4];
    const BtlAttackMotionSet *set;
    const BtlArtSeqStep *steps;
    const VtblEntry *entry;
    const BtlHitWindowDef *def;
    BtlBakugan *target;
    bool hadTarget;
    bool phaseStarted;
    bool stepDone;
    bool lastStep;
    s32 next;
    float threshold;
    float dist;
    float speed;
    float yaw;
    float oldSpeed;
    float timeScale;
    float vy;
    float len2;
    float k;
    float ox;
    float oz;
    float c;
    float s;
    int i;

    hadTarget = false;
    if (BtlBakuganGetTarget(self) != NULL && self->stateCounter == 0) {
        hadTarget = true;
    }
    set = (const BtlAttackMotionSet *)self->attackMotions[self->attackIndex];
    steps = g_btlKindArtSequences[self->base.base.unk08];
    threshold = 0.980000019f;
    BtlBakuganTurnTowardTarget(self, NULL);
    self->stateFlags |= 0x400000;
    phaseStarted = false;
    target = (BtlBakugan *)BtlBakuganGetTarget(self);

    switch ((s32)self->attackPhase) {
    case 0:
        if (set != NULL) {
            if ((set->flags & 2) != 0) {
                self->stateFlags |= 0x8000000;
            }
            if ((set->flags & 0x40) != 0) {
                threshold = 0.699999988f;
            }
        }
        if (GfxModelMotionReached(&self->base, threshold)) {
            phaseStarted = true;
            BtlBakuganPlayAttackMotion(self, 1);
        }
        break;

    case 1:
        if ((self->stateFlags & 0x2000) != 0 && BtlBakuganGetTarget(self) != NULL) {
            self->collider0->hitTimer = 10;
            self->collider0->flags |= 1;
            self->grabTarget = target;
            if ((set->flags & 0x20000) != 0) {
                self->collider0->hitTimer = 999;
                self->collider0->flags |= 1;
                BtlBakuganSetState(self, 0x14, 0);
                self->stateFlags &= ~0x2000u;
                return;
            }
            self->attackPhase = 2;
            self->subTimer = 0;
            self->subWait = 0;
            self->stateCounter = 0;
            self->stateTicks = 0;
            BtlBakuganPlayMotion(0.0670000017f, self, steps[0].motion, 0, 0);
            entry = &((const VtblEntry *)self->base.base.vtable)[6];
            ((BtlSetMotionSpeedFn)entry->fn)(steps[self->subTimer].speed,
                                             (u8 *)self + entry->delta);
        } else {
            self->collider0->hitTimer = 0;
            self->collider0->flags &= ~1u;
            if (self->subWait++ >= 0x13) {
                BtlBakuganPlayAttackMotion(self, 3);
            } else {
                BtlBakuganStartColorFlash(0.800000012f, self, BtlBakuganGetAttributeColor(self));
            }
        }
        self->stateFlags |= 0x1000000;
        if (BtlBakuganIsLocalPlayer(self)) {
            /* dir = normalised velocity */
            len2 = self->base.velocity[0] * self->base.velocity[0] +
                   self->base.velocity[1] * self->base.velocity[1] +
                   self->base.velocity[2] * self->base.velocity[2];
            k = (len2 == 0.0f) ? 0.0f : VfRsq(len2);
            for (i = 0; i < 3; i++) {
                dir[i] = VfSat1(self->base.velocity[i] * k);
            }
            dir[3] = 0.0f;
            GfxEffectSpawnDirected(g_worldEffectMgr, 0x1d, self->base.pos, dir);
            GfxEffectSpawnDirected(g_worldEffectMgr, 0x1d, self->base.pos, dir);
        }
        break;

    case 2:
        self->collider0->hitTimer = 10;
        self->collider0->flags |= 1;
        if (self->stateTicks == 0) {
            self->stateTicks++;
            if (BtlMainStartCutIn((BtlMain *)BtlGetCameraTask(), self) != 0) {
                self->flags |= 0x400;
            }
        }
        if (set != NULL && (set->flags & 2) != 0) {
            self->stateFlags |= 0x8000000;
        }
        self->comboTimer = 0x28;
        if (self->subWait == 0 && (steps[self->subTimer].flags & 8) != 0 &&
            GfxModelMotionReached(&self->base, 0.479999989f)) {
            BtlBakuganStartHitWindow(self, &g_btlArtHitWindows[0]);
            self->subWait = 1;
        }
        stepDone = false;
        if (GfxModelMotionReached(&self->base, 0.949999988f)) {
            if (self->stateCounter != 0) {
                stepDone = true;
            } else if ((steps[self->subTimer].flags & 0x10) == 0) {
                def = &g_btlArtHitWindows[0];
                if ((steps[self->subTimer].flags & 7) != 0) {
                    if ((steps[self->subTimer].flags & 2) != 0) {
                        def = &g_btlArtHitWindows[2];
                    } else if ((steps[self->subTimer].flags & 4) != 0) {
                        def = &g_btlArtHitWindows[3];
                    } else {
                        def = &g_btlArtHitWindows[1];
                    }
                    self->stateCounter = 1;
                }
                BtlBakuganStartHitWindow(self, (BtlHitWindowDef *)def);
            }
            if ((steps[self->subTimer].flags & 0x20) != 0) {
                self->collider0->hitTimer = 0;
                self->collider0->flags &= ~1u;
                self->stateFlags &= ~0x100u;
                BtlBakuganSetState(self, 0, 0);
                entry = &((const VtblEntry *)self->base.base.vtable)[26];
                ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
                if ((self->flags & 0x400) != 0) {
                    BtlGetCameraTask();
                    BtlEndCutIn();
                    self->flags &= ~0x400u;
                }
                self->stateFlags &= ~0x2000u;
            } else {
                next = ++self->subTimer;
                BtlBakuganPlayMotion(0.0670000017f, self, steps[next].motion, 0, 0);
                entry = &((const VtblEntry *)self->base.base.vtable)[6];
                ((BtlSetMotionSpeedFn)entry->fn)(steps[self->subTimer].speed,
                                                 (u8 *)self + entry->delta);
                self->subWait = 0;
                if ((steps[self->subTimer].flags & 7) != 0 &&
                    BtlBakuganTargetInvolvesPlayer(self) != 0) {
                    GfxSetMotionTimeScale(0.5f);
                    ((BtlMain *)BtlGetCameraTask())->flashTarget = 0.800000012f;
                }
            }
        }
        if ((self->stateCounter != 0 || stepDone) && self->stateTicks++ == 0x15) {
            self->stateFlags &= ~0x2000u;
            if ((self->flags & 0x400) != 0) {
                BtlGetCameraTask();
                BtlEndCutIn();
                self->flags &= ~0x400u;
            }
            if (BtlBakuganTargetInvolvesPlayer(self) != 0) {
                GfxSetMotionTimeScale(1.0f);
                ((BtlMain *)BtlGetCameraTask())->flashTarget = 0.0f;
            }
        }
        if (GfxModelGetMotionRemaining(&self->base) < 3.0f &&
            (steps[self->subTimer].flags & 7) != 0 && BtlBakuganTargetInvolvesPlayer(self) != 0) {
            GfxSetMotionTimeScale(0.649999976f);
        }
        break;

    case 3:
        if (GfxModelMotionReached80(&self->base)) {
            self->collider0->hitTimer = 0;
            self->collider0->flags &= ~1u;
            self->stateFlags &= ~0x100u;
            BtlBakuganSetState(self, 0, 0);
            entry = &((const VtblEntry *)self->base.base.vtable)[26];
            ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
            if ((self->flags & 0x400) != 0) {
                BtlGetCameraTask();
                BtlEndCutIn();
                self->flags &= ~0x400u;
            }
            if (BtlBakuganTargetInvolvesPlayer(self) != 0) {
                GfxSetMotionTimeScale(1.0f);
                ((BtlMain *)BtlGetCameraTask())->flashTarget = 0.0f;
            }
            self->stateFlags &= ~0x2000u;
            if (hadTarget && target->lockedAttacker == self) {
                target->lockedAttacker = NULL;
            }
        }
        break;

    default:
        break;
    }

    if ((s32)self->attackPhase > 0 && (s32)self->attackPhase < 3) {
        self->stateFlags |= 1;
        /* norm = normalised velocity (only y is used) */
        len2 = self->base.velocity[0] * self->base.velocity[0] +
               self->base.velocity[1] * self->base.velocity[1] +
               self->base.velocity[2] * self->base.velocity[2];
        k = (len2 == 0.0f) ? 0.0f : VfRsq(len2);
        for (i = 0; i < 3; i++) {
            norm[i] = VfSat1(self->base.velocity[i] * k);
        }
        norm[3] = 0.0f;
        self->orient[0] = norm[1] * -0.899999976f;
        self->orient[1] = 1.0f;
        self->orient[2] = 0.0f;
        self->orient[3] = 0.0f;
        /* orient: rotate xz by rot.y (vrot of rot.y * 2/pi), then normalise */
        c = __builtin_cosf(self->base.rot[1]);
        s = __builtin_sinf(self->base.rot[1]);
        ox = self->orient[0];
        oz = self->orient[2];
        self->orient[0] = ox * c + self->orient[1] * 0.0f + oz * -s;
        self->orient[2] = ox * s + self->orient[1] * 0.0f + oz * c;
        len2 = self->orient[0] * self->orient[0] + self->orient[1] * self->orient[1] +
               self->orient[2] * self->orient[2];
        k = (len2 == 0.0f) ? 0.0f : VfRsq(len2);
        for (i = 0; i < 3; i++) {
            self->orient[i] = VfSat1(self->orient[i] * k);
        }
        self->orient[3] = 0.0f;
    }
    if ((s32)self->attackPhase < 2) {
        BtlBakuganSetStateFlag04(self);
    }
    if ((s32)self->attackPhase < 1) {
        self->stateFlags |= 0x2000000;
    }

    lastStep = false;
    if (set != NULL && (set->flags & 0x20) != 0) {
        lastStep = true;
    }
    if (self->attackPhase == 0 && !lastStep) {
        if (set != NULL && (set->flags & 1) != 0) {
            vy = self->groundY + 110.0f - self->base.pos[1];
            if (vy < 0.0f) {
                vy = 0.0f;
            }
            self->base.velocity[1] = self->gravity + vy * 0.0500000007f;
        }
    } else if ((self->attackPhase == 2 || self->attackPhase == 1 || lastStep) &&
               self->stateCounter == 0 && (set == NULL || (set->flags & 0x80) == 0)) {
        if (hadTarget) {
            /* delta.xyz = target pos - own pos (w kept from the target) */
            for (i = 0; i < 4; i++) {
                delta[i] = target->base.pos[i];
            }
            for (i = 0; i < 3; i++) {
                delta[i] = delta[i] - self->base.pos[i];
            }
            if (phaseStarted) {
                speed = BtlBakuganGetScaledStat48(self);
                self->dashSpeed = speed;
                /* velocity = normalise(delta.xyz) * speed, w 0 */
                len2 = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
                k = (len2 == 0.0f) ? 0.0f : VfRsq(len2);
                k = k * speed;
                for (i = 0; i < 3; i++) {
                    delta[i] = delta[i] * k;
                }
                delta[3] = 0.0f;
                for (i = 0; i < 4; i++) {
                    self->base.velocity[i] = delta[i];
                }
            } else {
                dist = __builtin_sqrtf(delta[0] * delta[0] + delta[1] * delta[1] +
                                       delta[2] * delta[2]);
                dist = dist - self->combat.stats->reachRadius * 1.20000005f;
                speed = self->dashSpeed;
                if (dist < 0.0f) {
                    dist = 0.0f;
                } else if (speed < dist) {
                    dist = speed;
                }
                /* delta = normalise(delta.xyz) * dist (w 0); velocity += (delta - velocity) * 0.3 */
                len2 = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
                k = (len2 == 0.0f) ? 0.0f : VfRsq(len2);
                k = k * dist;
                for (i = 0; i < 3; i++) {
                    delta[i] = delta[i] * k;
                }
                delta[3] = 0.0f;
                for (i = 0; i < 4; i++) {
                    self->base.velocity[i] =
                        self->base.velocity[i] +
                        (delta[i] - self->base.velocity[i]) * 0.300000012f;
                }
                oldSpeed = self->dashSpeed;
                timeScale = GfxGetMotionTimeScale();
                speed = BtlBakuganGetScaledStat4C(self);
                self->dashSpeed = oldSpeed + (speed - self->dashSpeed) * 0.100000001f * timeScale;
            }
        } else {
            yaw = self->base.rot[1];
            speed = BtlBakuganGetMeleeStepSpeed(self);
            /* step = {cos, 0, sin}(yaw) * speed, w 0; velocity += (step - velocity) * 0.3 */
            step[0] = __builtin_cosf(yaw) * speed;
            step[1] = 0.0f * speed;
            step[2] = __builtin_sinf(yaw) * speed;
            step[3] = 0.0f;
            for (i = 0; i < 4; i++) {
                self->base.velocity[i] =
                    self->base.velocity[i] + (step[i] - self->base.velocity[i]) * 0.300000012f;
            }
        }
    }

    if (!hadTarget) {
        /* velocity.x/z *= 0.85 */
        self->base.velocity[0] = self->base.velocity[0] * 0.850000024f;
        self->base.velocity[2] = self->base.velocity[2] * 0.850000024f;
    }
    self->attackFrame++;
    if ((s32)self->attackPhase >= 2) {
        self->attackEventFrame++;
    }
}
