// bdc 0x0886ebcc BtlBakuganState20Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 20 (`+0x140`), vtable slot `+0x170` called through
   `BtlBakuganRunState`: the multi-pass dash at `grabTarget`, stepped by `subTimer`.
   When `grabTarget` is no longer in the unit list (`BtlBakuganListFind`) it clears `flags` bit 3,
   sets the ambient alpha to 1, re-enables `collider0` (flags bit 0 off, `hitTimer` 0), clears
   `stateFlags` 0x100, enters state 0 (`BtlBakuganSetState`) and runs the state-0 handler
   (virtual slot `+0xd0`) at once, then returns.
   0: alpha x0.9, `statePos` = anchor translation, `effectAnchor` = unit velocity direction,
   velocity zeroed, `dashSpeed` 200, `stateCounter` 1, `subWait` 15, `dashHeading` 0,
   `stateTicks` 0, `flags` |= 8, then 1.
   1: `dashHeading` ramps to 1 by 1/16; with e = (1 - cos(dashHeading x pi)) / 2 velocity.y
   = (target y + 40 - y) x 0.2 x e and, beyond the summed `reachRadius`, the velocity gains the
   horizontal direction to the target x e x 0.01 x excess. Alpha -0.3 (clamped to 0..1),
   `statePos` += `effectAnchor` x `dashSpeed`. When `stateCounter` passes `subWait`, `subWait`
   shrinks to 2, then each pass counts `stateTicks`: below 5 a new pass starts (`stateCounter` 0,
   direction from `statePos` to the target anchor flattened, turned about its horizontal normal by
   a random 10..40 degrees and about y by 10 degrees, `statePos` = target anchor - 500 x direction,
   directed world effect 0xf3 there with an orientation matrix); at 5 the velocity becomes 50 units
   horizontally away from the target, the heading follows it (`BtlBakuganSetHeading`), alpha
   0.4, `flags` bit 3 off and 2. The frame where `stateCounter` reaches 1 runs the hit test.
   2: alpha eases to 1 (rate 0.1), velocity x0.9; below speed 5 alpha 1, `dashSpeed` 0, motion 0xd
   (`BtlBakuganPlayMotion`), `stateCounter`/`stateTicks` 0, then 3.
   3: velocity x0.9; at `stateCounter` 14 one combo hit (`BtlBakuganAddComboHits`); at 18 effect
   0xa4 at the target, `effectAnchor` = direction to it, hit test, `comboTimer` 60; at 30 collider
   restored, state 0, the target's `lockedAttacker` cleared if it is this unit, state-0 handler run.
   States 0..3 keep `gravityHold` at 10.
   Hit test: `CollisionTestPair` of a query (attack 0x86, 0x87 in sub-step 3 with
   `g_collisionAttackFlags` 1; flags 60 once `stateTicks` >= 4; heading atan2 of `effectAnchor`)
   against the target's `collider0`, with the sphere `g_btlBakuganSphereQuery` (radius 500)
   centred 100 units back along `effectAnchor` from the target anchor and this unit's anchor moved
   there for the test; afterwards `collider0->cooldown` 0 and sound 0x200099
   (`SndObjectAddEmitter`).
   VFPU bank constants (S703 2/pi, S713/S730 0, S733 1, C720 zero) are written as literals. */

void BtlBakuganState20Update(BtlBakugan *self)
{
    struct {
        CollisionQuery q;
        s32 tail[3];
    } query;
    float diff[3];
    float axis[4];
    float quat[4];
    float conj[4];
    float vec[4];
    float rot[4];
    float side[3];
    float up[3];
    float dir[3];
    float saved[4];
    float *m;
    BtlBakugan *other;
    const VtblEntry *entry;
    GfxEffect *fx;
    float t;
    float c;
    float s;
    float dist;
    float reach;
    float len2;
    float inv;
    float scale;
    float alpha;
    float angle;
    u8 hit;

    hit = 0;
    other = (BtlBakugan *)BtlBakuganListFind(self->grabTarget);
    if (other == NULL) {
        self->flags &= ~8u;
        self->base.ambient[3] = 1.0f;
        self->collider0->flags &= ~1u;
        self->collider0->hitTimer = 0;
        self->stateFlags &= ~0x100u;
        BtlBakuganSetState(self, 0, 0);
        entry = &((const VtblEntry *)self->base.base.vtable)[26];
        ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
        return;
    }

    if (self->subTimer < 2) {
        if (self->subTimer < 0) {
            goto hit_test;
        }
        if (self->subTimer <= 0) {
            self->base.ambient[3] = self->base.ambient[3] * 0.9f;
            /* statePos = anchor translation */
            self->statePos[0] = self->anchorMatrix[3][0];
            self->statePos[1] = self->anchorMatrix[3][1];
            self->statePos[2] = self->anchorMatrix[3][2];
            self->statePos[3] = self->anchorMatrix[3][3];
            /* effectAnchor = unit velocity (0 for a zero velocity), lanes clamped to [-1, 1], w 0 */
            len2 = self->base.velocity[0] * self->base.velocity[0] +
                   self->base.velocity[1] * self->base.velocity[1] +
                   self->base.velocity[2] * self->base.velocity[2];
            inv = len2 == 0.0f ? 0.0f : VfRsq(len2);
            self->effectAnchor[0] = VfSat1(self->base.velocity[0] * inv);
            self->effectAnchor[1] = VfSat1(self->base.velocity[1] * inv);
            self->effectAnchor[2] = VfSat1(self->base.velocity[2] * inv);
            self->effectAnchor[3] = 0.0f;
            self->base.velocity[0] = 0.0f;
            self->base.velocity[1] = 0.0f;
            self->base.velocity[2] = 0.0f;
            self->base.velocity[3] = 0.0f;
            self->dashSpeed = 200.0f;
            self->subTimer = self->subTimer + 1;
            self->stateCounter = 1;
            self->subWait = 15;
            self->dashHeading = 0.0f;
            self->stateTicks = 0;
            self->gravityHold = 10;
            self->flags |= 8;
            goto hit_test;
        }

        t = self->dashHeading + 0.0625f;
        if (!(t <= 1.0f)) {
            t = 1.0f;
        }
        self->dashHeading = t;
        c = __builtin_cosf(t * 3.14159274f);
        self->base.velocity[1] =
            ((other->base.pos[1] + 40.0f) - self->base.pos[1]) * 0.2f * ((1.0f - c) * 0.5f);
        /* diff = target anchor - own anchor; dist = |diff| */
        diff[0] = other->anchorMatrix[3][0] - self->anchorMatrix[3][0];
        diff[1] = other->anchorMatrix[3][1] - self->anchorMatrix[3][1];
        diff[2] = other->anchorMatrix[3][2] - self->anchorMatrix[3][2];
        dist = __builtin_sqrtf(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
        reach = other->combat.stats->reachRadius + self->combat.stats->reachRadius;
        if (!(dist <= reach)) {
            c = __builtin_cosf(self->dashHeading * 3.14159274f);
            scale = (1.0f - c) * 0.5f * 0.01f * (dist - reach);
            /* diff = diff / |diff| * scale, flattened, added to the velocity */
            len2 = diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2];
            inv = len2 == 0.0f ? 0.0f : VfRsq(len2);
            inv = inv * scale;
            diff[0] = diff[0] * inv;
            diff[2] = diff[2] * inv;
            diff[1] = 0.0f;
            self->base.velocity[0] = self->base.velocity[0] + diff[0];
            self->base.velocity[1] = self->base.velocity[1] + diff[1];
            self->base.velocity[2] = self->base.velocity[2] + diff[2];
        }
        self->gravityHold = 10;
        /* vmin.s with 1, then vmax.s with 0: +NaN gives 1, -NaN gives 0, -0 gives +0 */
        alpha = self->base.ambient[3] - 0.3f;
        if (alpha != alpha) {
            alpha = __builtin_signbit(alpha) ? 0.0f : 1.0f;
        } else {
            alpha = alpha <= 0.0f ? 0.0f : alpha > 1.0f ? 1.0f : alpha;
        }
        self->base.ambient[3] = alpha;
        scale = self->dashSpeed;
        self->statePos[0] = self->statePos[0] + self->effectAnchor[0] * scale;
        self->statePos[1] = self->statePos[1] + self->effectAnchor[1] * scale;
        self->statePos[2] = self->statePos[2] + self->effectAnchor[2] * scale;
        self->stateCounter = self->stateCounter + 1;
        if (!(self->subWait < self->stateCounter)) {
            if (self->stateCounter == 1) {
                hit = 1;
            }
            goto hit_test;
        }
        if (self->subWait < 3) {
            self->stateTicks = self->stateTicks + 1;
        } else {
            self->subWait = self->subWait - 1;
        }
        if (self->stateTicks < 5) {
            self->stateCounter = 0;
            /* effectAnchor = target anchor - statePos (w = target anchor w), flattened */
            self->effectAnchor[0] = other->anchorMatrix[3][0] - self->statePos[0];
            self->effectAnchor[1] = other->anchorMatrix[3][1] - self->statePos[1];
            self->effectAnchor[2] = other->anchorMatrix[3][2] - self->statePos[2];
            self->effectAnchor[3] = other->anchorMatrix[3][3];
            self->effectAnchor[1] = 0.0f;
            /* normalise (lanes clamped to [-1, 1], w 0) */
            len2 = self->effectAnchor[0] * self->effectAnchor[0] +
                   self->effectAnchor[1] * self->effectAnchor[1] +
                   self->effectAnchor[2] * self->effectAnchor[2];
            inv = len2 == 0.0f ? 0.0f : VfRsq(len2);
            self->effectAnchor[0] = VfSat1(self->effectAnchor[0] * inv);
            self->effectAnchor[1] = VfSat1(self->effectAnchor[1] * inv);
            self->effectAnchor[2] = VfSat1(self->effectAnchor[2] * inv);
            self->effectAnchor[3] = 0.0f;
            /* axis = (z, y, -x, w): the horizontal normal of the direction */
            axis[0] = self->effectAnchor[2];
            axis[1] = self->effectAnchor[1];
            axis[2] = -self->effectAnchor[0];
            axis[3] = self->effectAnchor[3];
            angle = CoreRandFloat(0.52359879f) + 0.17453292f;
            /* quat = (axis * sin(angle / 2), cos(angle / 2)); vcst 1/pi times angle in quarter turns */
            t = 0.318309873f * angle;
            c = VfCosQuarter(t);
            s = VfSinQuarter(t);
            quat[0] = axis[0] * s;
            quat[1] = axis[1] * s;
            quat[2] = axis[2] * s;
            quat[3] = c;
            /* effectAnchor = (quat * (effectAnchor.xyz, 0)) * conj(quat) */
            conj[0] = -quat[0];
            conj[1] = -quat[1];
            conj[2] = -quat[2];
            conj[3] = quat[3];
            vec[0] = self->effectAnchor[0];
            vec[1] = self->effectAnchor[1];
            vec[2] = self->effectAnchor[2];
            vec[3] = 0.0f;
            rot[0] = quat[0] * vec[3] + quat[1] * vec[2] - quat[2] * vec[1] + quat[3] * vec[0];
            rot[1] = -quat[0] * vec[2] + quat[1] * vec[3] + quat[2] * vec[0] + quat[3] * vec[1];
            rot[2] = quat[0] * vec[1] - quat[1] * vec[0] + quat[2] * vec[3] + quat[3] * vec[2];
            rot[3] = -quat[0] * vec[0] - quat[1] * vec[1] - quat[2] * vec[2] + quat[3] * vec[3];
            self->effectAnchor[0] =
                rot[0] * conj[3] + rot[1] * conj[2] - rot[2] * conj[1] + rot[3] * conj[0];
            self->effectAnchor[1] =
                -rot[0] * conj[2] + rot[1] * conj[3] + rot[2] * conj[0] + rot[3] * conj[1];
            self->effectAnchor[2] =
                rot[0] * conj[1] - rot[1] * conj[0] + rot[2] * conj[3] + rot[3] * conj[2];
            self->effectAnchor[3] =
                -rot[0] * conj[0] - rot[1] * conj[1] - rot[2] * conj[2] + rot[3] * conj[3];
            /* rotate about y by 10 degrees (vrot rows (c, 0, -s) and (s, 0, c)) */
            c = __builtin_cosf(0.17453292f);
            s = __builtin_sinf(0.17453292f);
            vec[0] = self->effectAnchor[0];
            vec[1] = self->effectAnchor[1];
            vec[2] = self->effectAnchor[2];
            self->effectAnchor[0] = vec[0] * c + vec[1] * 0.0f + vec[2] * -s;
            self->effectAnchor[2] = vec[0] * s + vec[1] * 0.0f + vec[2] * c;
            /* statePos = target anchor - effectAnchor * 500 (w = target anchor w) */
            self->statePos[0] = other->anchorMatrix[3][0] - self->effectAnchor[0] * 500.0f;
            self->statePos[1] = other->anchorMatrix[3][1] - self->effectAnchor[1] * 500.0f;
            self->statePos[2] = other->anchorMatrix[3][2] - self->effectAnchor[2] * 500.0f;
            self->statePos[3] = other->anchorMatrix[3][3];
            fx = GfxEffectSpawnDirected(g_worldEffectMgr, 0xf3, self->statePos, self->effectAnchor);
            /* fx->matrix rows: side = unit(up x dir), up' = dir x side, dir = unit(effectAnchor)
               (lanes clamped to [-1, 1]), w column 0, translation row (0, 0, 0, 1) */
            len2 = self->effectAnchor[0] * self->effectAnchor[0] +
                   self->effectAnchor[1] * self->effectAnchor[1] +
                   self->effectAnchor[2] * self->effectAnchor[2];
            inv = len2 == 0.0f ? 0.0f : VfRsq(len2);
            dir[0] = VfSat1(self->effectAnchor[0] * inv);
            dir[1] = VfSat1(self->effectAnchor[1] * inv);
            dir[2] = VfSat1(self->effectAnchor[2] * inv);
            up[0] = g_vecUp.x;
            up[1] = g_vecUp.y;
            up[2] = g_vecUp.z;
            side[0] = up[1] * dir[2] - up[2] * dir[1];
            side[1] = up[2] * dir[0] - up[0] * dir[2];
            side[2] = up[0] * dir[1] - up[1] * dir[0];
            len2 = side[0] * side[0] + side[1] * side[1] + side[2] * side[2];
            inv = len2 == 0.0f ? 0.0f : VfRsq(len2);
            side[0] = VfSat1(side[0] * inv);
            side[1] = VfSat1(side[1] * inv);
            side[2] = VfSat1(side[2] * inv);
            up[0] = dir[1] * side[2] - dir[2] * side[1];
            up[1] = dir[2] * side[0] - dir[0] * side[2];
            up[2] = dir[0] * side[1] - dir[1] * side[0];
            m = fx->matrix;
            m[0] = side[0];
            m[1] = side[1];
            m[2] = side[2];
            m[3] = 0.0f;
            m[4] = up[0];
            m[5] = up[1];
            m[6] = up[2];
            m[7] = 0.0f;
            m[8] = dir[0];
            m[9] = dir[1];
            m[10] = dir[2];
            m[11] = 0.0f;
            m[12] = 0.0f;
            m[13] = 0.0f;
            m[14] = 0.0f;
            m[15] = 1.0f;
            goto hit_test;
        }
        /* velocity = own pos - target pos, flattened, scaled to 50 (w 0) */
        self->base.velocity[0] = self->base.pos[0] - other->base.pos[0];
        self->base.velocity[1] = self->base.pos[1] - other->base.pos[1];
        self->base.velocity[2] = self->base.pos[2] - other->base.pos[2];
        self->base.velocity[3] = self->base.pos[3];
        self->base.velocity[1] = 0.0f;
        len2 = self->base.velocity[0] * self->base.velocity[0] +
               self->base.velocity[1] * self->base.velocity[1] +
               self->base.velocity[2] * self->base.velocity[2];
        inv = len2 == 0.0f ? 0.0f : VfRsq(len2);
        inv = inv * 50.0f;
        self->base.velocity[0] = self->base.velocity[0] * inv;
        self->base.velocity[1] = self->base.velocity[1] * inv;
        self->base.velocity[2] = self->base.velocity[2] * inv;
        self->base.velocity[3] = 0.0f;
        self->subTimer = self->subTimer + 1;
        BtlBakuganSetHeading(self, atan2f(self->base.velocity[2], self->base.velocity[0]));
        self->base.ambient[3] = 0.4f;
        self->flags &= ~8u;
        goto hit_test;
    }

    if (self->subTimer < 3) {
        self->base.ambient[3] = self->base.ambient[3] + (1.0f - self->base.ambient[3]) * 0.1f;
        self->base.velocity[0] = self->base.velocity[0] * 0.9f;
        self->base.velocity[1] = self->base.velocity[1] * 0.9f;
        self->base.velocity[2] = self->base.velocity[2] * 0.9f;
        self->base.velocity[3] = 0.0f;
        self->gravityHold = 10;
        t = self->base.velocity[0] * self->base.velocity[0] +
            self->base.velocity[1] * self->base.velocity[1] +
            self->base.velocity[2] * self->base.velocity[2];
        if (t < 25.0f) {
            self->base.ambient[3] = 1.0f;
            self->dashSpeed = 0.0f;
            BtlBakuganPlayMotion(0.2f, self, 0xd, 0, 0);
            self->stateCounter = 0;
            self->subTimer = self->subTimer + 1;
            self->stateTicks = 0;
        }
        goto hit_test;
    }

    if (self->subTimer < 4) {
        self->base.velocity[0] = self->base.velocity[0] * 0.9f;
        self->base.velocity[1] = self->base.velocity[1] * 0.9f;
        self->base.velocity[2] = self->base.velocity[2] * 0.9f;
        self->base.velocity[3] = 0.0f;
        self->gravityHold = 10;
        self->stateCounter = self->stateCounter + 1;
        if (self->stateCounter == 18) {
            GfxEffectSpawn(g_worldEffectMgr, 0xa4, other->base.pos);
            /* effectAnchor = unit(target pos - own pos), lanes clamped to [-1, 1], w 0 */
            self->effectAnchor[0] = other->base.pos[0] - self->base.pos[0];
            self->effectAnchor[1] = other->base.pos[1] - self->base.pos[1];
            self->effectAnchor[2] = other->base.pos[2] - self->base.pos[2];
            len2 = self->effectAnchor[0] * self->effectAnchor[0] +
                   self->effectAnchor[1] * self->effectAnchor[1] +
                   self->effectAnchor[2] * self->effectAnchor[2];
            inv = len2 == 0.0f ? 0.0f : VfRsq(len2);
            self->effectAnchor[0] = VfSat1(self->effectAnchor[0] * inv);
            self->effectAnchor[1] = VfSat1(self->effectAnchor[1] * inv);
            self->effectAnchor[2] = VfSat1(self->effectAnchor[2] * inv);
            self->effectAnchor[3] = 0.0f;
            hit = 1;
            self->comboTimer = 60;
        } else if (self->stateCounter == 30) {
            self->collider0->hitTimer = 0;
            self->collider0->flags &= ~1u;
            self->stateFlags &= ~0x100u;
            BtlBakuganSetState(self, 0, 0);
            if (other->lockedAttacker == self) {
                other->lockedAttacker = NULL;
            }
            entry = &((const VtblEntry *)self->base.base.vtable)[26];
            ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
        }
        if (self->stateCounter == 14) {
            BtlBakuganAddComboHits(self, 1);
        }
    }

hit_test:
    if (hit == 0) {
        return;
    }
    query.q.contact.x = 0.0f;
    query.q.contact.y = 0.0f;
    query.q.contact.z = 0.0f;
    query.q.contact.w = 0.0f;
    query.q.hitCollider = NULL;
    query.q.shape = &g_collisionSphereBlock;
    query.q.owner = NULL;
    query.q.ownerCollider = NULL;
    query.q.hitParam = 1;
    query.q.heading = 0.0f;
    query.q.attackId = 0x86;
    query.q.attackSide = 1;
    query.q.flags = 0;
    query.tail[0] = 0;
    query.tail[1] = 0;
    query.tail[2] = 0;
    query.q.contact = g_gfxVecZero;
    query.q.owner = self;
    query.q.ownerCollider = self->collider0;
    query.q.heading = atan2f(self->effectAnchor[2], self->effectAnchor[0]);
    g_btlBakuganSphereQuery.radius = 500.0f;
    /* saved = own anchor; sphere centre = target anchor - effectAnchor * 100 (w = target anchor
       w); own anchor = centre for the test */
    saved[0] = self->anchorMatrix[3][0];
    saved[1] = self->anchorMatrix[3][1];
    saved[2] = self->anchorMatrix[3][2];
    saved[3] = self->anchorMatrix[3][3];
    g_btlBakuganSphereQuery.center.x = other->anchorMatrix[3][0] - self->effectAnchor[0] * 100.0f;
    g_btlBakuganSphereQuery.center.y = other->anchorMatrix[3][1] - self->effectAnchor[1] * 100.0f;
    g_btlBakuganSphereQuery.center.z = other->anchorMatrix[3][2] - self->effectAnchor[2] * 100.0f;
    g_btlBakuganSphereQuery.center.w = other->anchorMatrix[3][3];
    self->anchorMatrix[3][0] = g_btlBakuganSphereQuery.center.x;
    self->anchorMatrix[3][1] = g_btlBakuganSphereQuery.center.y;
    self->anchorMatrix[3][2] = g_btlBakuganSphereQuery.center.z;
    self->anchorMatrix[3][3] = g_btlBakuganSphereQuery.center.w;
    g_btlBakuganSphereQuery.center.w =
        g_btlBakuganSphereQuery.radius * g_btlBakuganSphereQuery.radius;
    if (self->subTimer == 3) {
        query.q.attackId = 0x87;
        g_collisionAttackFlags = 1;
    }
    if (!(self->stateTicks < 4)) {
        g_collisionAttackFlags = 60;
    }
    CollisionTestPair(0xffffffffu, other->collider0, (float *)&query.q, 0);
    self->anchorMatrix[3][0] = saved[0];
    self->anchorMatrix[3][1] = saved[1];
    self->anchorMatrix[3][2] = saved[2];
    self->anchorMatrix[3][3] = saved[3];
    self->collider0->cooldown = 0;
    SndObjectAddEmitter(self->base.sound, 0x200099, 0, 0);
}
