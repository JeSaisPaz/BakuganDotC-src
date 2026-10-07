// bdc 0x08874708 BtlBakuganState05Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 5 (`+0x140`), vtable slot `+0xf8` called through
   `BtlBakuganRunState`. Knock-back state: sets `stateFlags` bits 0x4040000 plus the low motion
   id word, arms the hit collider (flag 1, `hitTimer` 8), counts `subWait`/`stateCounter`, applies
   gravity 3.4499998 to `velocity.y` and moves the unit (`BtlBakuganApplyVelocity`); kind sound
   0xb on frame 30. While `subTimer` is in [-49, -1] it counts down and pulls the horizontal
   velocity 10 % toward the collider's hit position; on reaching -50 it launches: random heading
   kick of 0.1, horizontal speed renormalised to 40, `velocity.y` 30; otherwise it damps x/z by
   0.83 and eases `velocity.y` toward `-subTimer`. It then tilts to the velocity
   (`BtlBakuganTiltToVelocity`) and eases `tiltStep` toward 0.5. A unit that may be knocked out
   (`BtlBakuganCanStartKnockOut`) sets focus point 30 and, after 12 frames while not rising
   (`velocity.y <= 0`), goes to state 6 and returns. Falling (`velocity.y < 0`) with `subWait`
   >= 151, the next-frame height under `groundPoint.y` or `stateFlags` bit 31 set lands: orient
   reset to `g_vecUp`, motion 0xf5 (0xf7 for `g_btlBakuganLandFlipMotionIds`), landing effect,
   state 4, effect anchor at the Bip01 node, landing sound, all effects on the anchor stopped.
   Otherwise the anchor follows Bip01 and every third frame spawns trail effect 0x28 along
   `g_btlBakuganTiltDir` (other frames update the trail's `dir`). The VFPU math uses the bank
   constants S703 = 2/π (angle scale for `vrot`) and S713 = 0 (zero-length fallback scale and
   stored w lane). */
void BtlBakuganState05Update(BtlBakugan *self)
{
    float vel[4] __attribute__((aligned(16)));
    float kick[4] __attribute__((aligned(16)));
    float focus[4] __attribute__((aligned(16)));
    ScePspFVector4 landPos __attribute__((aligned(16)));
    ScePspFVector4 trailPos __attribute__((aligned(16)));
    CollisionCollider *col;
    s32 frames;
    s32 pair0;
    s32 pair1;
    s32 timer;
    s32 motion;
    float angle;
    float lenSq;
    float scale;
    float vy;

    self->stateFlags |= 0x4040000;
    col = self->collider0;
    col->flags |= 1;
    col->hitTimer = 8;
    self->stateFlags |= self->motionIdPair[0];
    self->subWait = self->subWait + 1;
    self->stateCounter = self->stateCounter + 1;
    self->base.velocity[1] = self->base.velocity[1] - 3.4499998f;

    /* vel = velocity */
    vel[0] = self->base.velocity[0];
    vel[1] = self->base.velocity[1];
    vel[2] = self->base.velocity[2];
    vel[3] = self->base.velocity[3];
    BtlBakuganApplyVelocity(self, vel);

    frames = 0;
    if ((self->stateFlags & 0x40000000) != 0) {
        frames = self->airborneFrames + 1;
    }
    /* the listing re-reads the motion id pair and stores it back unchanged */
    pair1 = self->motionIdPair[1];
    pair0 = self->motionIdPair[0];
    self->airborneFrames = frames;
    self->motionIdPair[1] = pair1;
    self->motionIdPair[0] = pair0;
    if (self->stateCounter == 30) {
        BtlBakuganPlayKindSound(self, 0xb, 0, 0);
    }

    if (self->subTimer < 0 && self->subTimer >= -49) {
        timer = self->subTimer - 1;
        col = self->collider0;
        self->subTimer = timer;
        self->base.velocity[0] = self->base.velocity[0] + (col->hitPos.x - self->base.pos[0]) * 0.1f;
        self->base.velocity[2] = self->base.velocity[2] + (col->hitPos.z - self->base.pos[2]) * 0.1f;
        if (timer == -50) {
            self->base.velocity[1] = 0.0f;
            angle = CoreRandFloat(6.28f);
            /* kick = {cos, 0, sin, 0}(angle) * 0.1; velocity.xyz += kick.xyz;
               velocity.xyz = normalise(velocity.xyz) * 40 (scale 0 when zero length), .w = 0 */
            kick[0] = __builtin_cosf(angle) * 0.1f;
            kick[1] = 0.0f * 0.1f;
            kick[2] = __builtin_sinf(angle) * 0.1f;
            kick[3] = 0.0f;
            self->base.velocity[0] = self->base.velocity[0] + kick[0];
            self->base.velocity[1] = self->base.velocity[1] + kick[1];
            self->base.velocity[2] = self->base.velocity[2] + kick[2];
            lenSq = self->base.velocity[0] * self->base.velocity[0] +
                    self->base.velocity[1] * self->base.velocity[1] +
                    self->base.velocity[2] * self->base.velocity[2];
            scale = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
            scale = scale * 40.0f;
            self->base.velocity[0] = self->base.velocity[0] * scale;
            self->base.velocity[1] = self->base.velocity[1] * scale;
            self->base.velocity[2] = self->base.velocity[2] * scale;
            self->base.velocity[3] = 0.0f;
            self->base.velocity[1] = 30.0f;
        } else {
            /* velocity.x/z *= 0.83 */
            self->base.velocity[0] = self->base.velocity[0] * 0.83f;
            self->base.velocity[2] = self->base.velocity[2] * 0.83f;
            self->base.velocity[1] =
                self->base.velocity[1] + ((float)-self->subTimer - self->base.velocity[1]) * 0.25f;
            self->tiltStep = 0.0f;
        }
    }

    /* the listing keeps the callee's leftover v0, always &g_btlBakuganTiltDir, as the trail dir */
    BtlBakuganTiltToVelocity(self->tiltStep, 1.0f, self);
    self->tiltStep = self->tiltStep + (0.5f - self->tiltStep) * 0.1f;

    if (BtlBakuganCanStartKnockOut(self) != 0) {
        focus[0] = 0.0f;
        focus[1] = 0.0f;
        focus[2] = 0.0f;
        focus[3] = 0.5f;
        BtlMainSetFocusPointGlobal(30, focus);
        if (self->subWait >= 12 && self->base.velocity[1] <= 0.0f) {
            BtlBakuganSetState(self, 6, 0);
            return;
        }
    }

    vy = self->base.velocity[1];
    if (vy < 0.0f &&
        (self->subWait >= 151 ||
         self->base.pos[1] + self->base.velocity[1] * 0.8f < self->groundPoint[1] ||
         (self->stateFlags & 0x80000000) == 0x80000000)) {
        /* orient = g_vecUp */
        self->orient[0] = g_vecUp.x;
        self->orient[1] = g_vecUp.y;
        self->orient[2] = g_vecUp.z;
        self->orient[3] = g_vecUp.w;
        motion = 0xf5;
        if (self->motionIdPair[0] == g_btlBakuganLandFlipMotionIds[0] &&
            self->motionIdPair[1] == g_btlBakuganLandFlipMotionIds[1]) {
            motion = 0xf7;
        }
        BtlBakuganPlayMotion(0.0f, self, motion, 0, 0);
        BtlBakuganSpawnLandingEffect(self, 0);
        self->subWait = 0;
        BtlBakuganSetState(self, 4, 0);
        GfxModelGetNodeWorldPos(&self->base, &landPos, "Bip01");
        /* effectAnchor = landPos */
        self->effectAnchor[0] = landPos.x;
        self->effectAnchor[1] = landPos.y;
        self->effectAnchor[2] = landPos.z;
        self->effectAnchor[3] = landPos.w;
        BtlBakuganPlayLandingSound(self);
        GfxEffectStopAttached(g_worldEffectMgr, -1, self->effectAnchor);
        self->subTimer = 0;
        self->tiltStep = 0.0f;
        return;
    }

    GfxModelGetNodeWorldPos(&self->base, &trailPos, "Bip01");
    /* effectAnchor = trailPos */
    self->effectAnchor[0] = trailPos.x;
    self->effectAnchor[1] = trailPos.y;
    self->effectAnchor[2] = trailPos.z;
    self->effectAnchor[3] = trailPos.w;
    if (self->stateCounter % 3 == 0) {
        self->trailEffect = (GfxEffect *)GfxEffectSpawnAttachedDir(
            g_worldEffectMgr, 0x28, self->effectAnchor, (const float *)&g_btlBakuganTiltDir);
    } else if (self->trailEffect != NULL) {
        /* trailEffect->dir = g_btlBakuganTiltDir */
        self->trailEffect->dir[0] = g_btlBakuganTiltDir.x;
        self->trailEffect->dir[1] = g_btlBakuganTiltDir.y;
        self->trailEffect->dir[2] = g_btlBakuganTiltDir.z;
        self->trailEffect->dir[3] = g_btlBakuganTiltDir.w;
    }
}
