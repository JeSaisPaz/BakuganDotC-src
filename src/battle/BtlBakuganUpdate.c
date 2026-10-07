// bdc 0x08868670 BtlBakuganUpdate
#include "bdc.h"

/* Per-frame update of the battle Bakugan unit (vtable `0x08af1fa4` slot `+0x38`; subclasses chain
   to it). Returns at once while the all-units control lock (`g_btlControlLockAll`) holds a player
   unit and the battle main task is in phase 6. Otherwise reads the input/AI command flags
   (`BtlInputReadActions` into `commands`) once the battle has started, ticks the body collider
   (`CollisionColliderTickHit`; a new hit sets `motionDone`), records combo inputs, clears the
   per-frame flags, and while the collider's hit cooldown runs (and no new hit came in) only plays
   the hit-stop shake (`BtlBakuganUpdateHitShake` with matrix rebuild, re-keying the leg effects of
   kinds 10/0xd) and returns. The normal path counts down `hitStunTimer` (clearing the flinch and
   knock-down gauges), runs counter timing, the stats state record, respawn protection, the state
   handler (virtual `+0x180`, `BtlBakuganRunState`), gravity, clamps the position to the arena
   bounds (`BtlStageGetArenaBounds`, 0.8 × radius margin) outside state 0xf, then charge, combo,
   targeting, motion and model matrix, bone anchors (virtual `+0xb8`), combat tick, ground probe,
   shadow, motion events, the pending hit window (`BtlBakuganMotionHitCheck`), the hit reaction
   (virtual `+0xc0`, `BtlBakuganOnHit`) on a new hit, status visuals, footsteps and the sword blur.
   Unless bit 0 of `stateFlags` is set, `orient` eases 20 % towards `g_vecUp` and is renormalised
   (xyz clamped to [-1, 1], a zero-length vector becomes 0, and `orient[3]` is set to 0).
   Ends with the hit shake, the colour flash and the `artCancelTimer` countdown. */
void BtlBakuganUpdate(BtlBakugan *self)
{
    const VtblEntry *entry;
    const float *bounds;
    float margin;
    int i;

    self->motionDone = 0;
    if (g_btlControlLockAll != 0 && self->isPlayer != 0 && BtlCameraTaskExists() != 0 &&
        ((BtlMain *)BtlGetCameraTask())->phase == 6) {
        return;
    }
    if (BtlCameraTaskExists() != 0 && ((BtlMain *)BtlGetCameraTask())->battleStarted != 0) {
        self->commands = BtlInputReadActions(self->input);
    }
    if (CollisionColliderTickHit(&self->collider0->node)) {
        self->motionDone = 1;
    }
    self->hitQueueLocked = (self->collider0->flags & 1) != 0;

    if ((self->flags & 0x40) != 0 && (self->stateFlags & 0x400000) != 0 &&
        (self->commands & 0x10) != 0 && (self->commands & 0x20000) == 0 &&
        self->comboInputCount < 8) {
        u8 input = 1;

        if ((self->commands & 0x10000) != 0) {
            input = 2;
        }
        self->comboInputs[self->comboInputCount] = input;
        if (self->queuedComboSteps == 0) {
            self->comboInputCount++;
            self->queuedComboSteps++;
        }
    }
    self->stateFlags &= ~0xc0u;

    if (self->collider0->cooldown != 0 && self->motionDone == 0) {
        /* Hit-stop: shake only, the leg effects of kinds 10 and 0xd are re-keyed. */
        BtlBakuganUpdateHitShake(self, 1);
        if (self->base.base.unk08 == 10 && (self->stateFlags & 0x800000) == 0) {
            for (i = 0; i < 4; i++) {
                self->legs[i]->key = 1;
            }
        }
        if (self->base.base.unk08 == 0xd && (self->stateFlags & 0x800000) == 0) {
            for (i = 0; i < 2; i++) {
                self->legs[i]->key = 1;
            }
        }
        return;
    }

    self->stateFlags &= 0x40197fe8;
    self->flags &= ~0x260u;
    if (self->hitStunTimer != 0) {
        self->hitStunTimer--;
        if (self->hitStunTimer == 0) {
            self->flinchGauge = 0;
            self->knockdownGauge = 0;
        }
    }
    BtlBakuganUpdateCounterTiming(self);
    if (self->stats != NULL) {
        BtlStatsSetState(self->stats, self->state, self->commands);
    }
    BtlBakuganUpdateRespawnProtection(self);
    entry = &((const VtblEntry *)self->base.base.vtable)[48]; /* +0x180 BtlBakuganRunState */
    ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
    BtlBakuganApplyGravity(self);

    if (self->state != 0xf) {
        /* bounds = {min x, y, z, max x, y, z} */
        bounds = BtlStageGetArenaBounds();
        margin = self->radius * 0.8f;
        if (self->base.pos[0] - margin < bounds[0]) {
            self->base.pos[0] = margin + bounds[0];
        } else if (!(self->base.pos[0] + margin <= bounds[3])) {
            self->base.pos[0] = bounds[3] - margin;
        }
        if (self->base.pos[2] - margin < bounds[2]) {
            self->base.pos[2] = margin + bounds[2];
        } else if (!(self->base.pos[2] + margin <= bounds[5])) {
            self->base.pos[2] = bounds[5] - margin;
        }
    }

    BtlBakuganUpdateCharge(self);
    BtlBakuganUpdateCombo(self);
    BtlBakuganUpdateTargeting(self);
    GfxModelUpdateMotion(&self->base);
    GfxModelApplyMotion(&self->base);
    BtlBakuganUpdateModelMatrix(self);
    entry = &((const VtblEntry *)self->base.base.vtable)[23]; /* +0xb8 BtlBakuganUpdateBoneAnchors */
    ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
    BtlBakuganTickCombat(self);
    BtlBakuganProbeGround(self);
    self->groundProbed = 0;
    BtlShadowUpdate(self->shadow);
    if ((self->stateFlags & 4) != 0) {
        BtlBakuganProcessMotionEvents(self);
    }
    if (self->hitWindowActive != 0) {
        BtlBakuganMotionHitCheck(self, &self->hitWindowActive);
        self->hitWindowActive = 0;
    }
    if (self->motionDone != 0) {
        entry = &((const VtblEntry *)self->base.base.vtable)[24]; /* +0xc0 BtlBakuganOnHit */
        ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
    }
    BtlBakuganUpdateStatusVisuals(self);
    BtlBakuganUpdateFootsteps(self);
    if (self->weapon != NULL) {
        ((BtlSwordBlur *)self->weapon)->active = (self->stateFlags & 0x8000000) != 0;
        BtlSwordBlurUpdate(self->weapon);
    }

    if ((self->stateFlags & 1) == 0) {
        /* orient += (up - orient) * 0.2; then orient.xyz = clamp(orient.xyz / |orient.xyz|, -1, 1)
           (a zero length gives 0), orient.w = 0 (bank S713, the masked lane of C710). */
        float lenSq;
        float inv;

        self->orient[0] = self->orient[0] + (g_vecUp.x - self->orient[0]) * 0.2f;
        self->orient[1] = self->orient[1] + (g_vecUp.y - self->orient[1]) * 0.2f;
        self->orient[2] = self->orient[2] + (g_vecUp.z - self->orient[2]) * 0.2f;
        self->orient[3] = self->orient[3] + (g_vecUp.w - self->orient[3]) * 0.2f;
        lenSq = self->orient[0] * self->orient[0] + self->orient[1] * self->orient[1] +
                self->orient[2] * self->orient[2];
        inv = VfRsq(lenSq);
        if (lenSq == 0.0f) {
            inv = 0.0f;
        }
        self->orient[0] = VfSat1(self->orient[0] * inv);
        self->orient[1] = VfSat1(self->orient[1] * inv);
        self->orient[2] = VfSat1(self->orient[2] * inv);
        self->orient[3] = 0.0f;
    }

    BtlBakuganUpdateHitShake(self, 0);
    BtlBakuganUpdateColorFlash(self);
    if (self->artCancelTimer != 0) {
        self->artCancelTimer--;
    }
}
