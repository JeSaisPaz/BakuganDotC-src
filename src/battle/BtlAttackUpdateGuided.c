// bdc 0x08878e1c BtlAttackUpdateGuided
#include "bdc.h"

/* Shared body of the guided projectile types (`BtlAttackType03Update`, `BtlAttackType0FUpdate`,
   `BtlAttackType13Update`, `BtlAttackType17Update`, `BtlAttackType20Update`,
   `BtlAttackType45Update`, `BtlAttackUpdateSideGuidedShot`, `BtlAttackType2CUpdate`,
   `BtlAttackType5CUpdate`, ...). From frame 0x79 it ends (`BtlAttackEnd`) and returns 1.
   Unless the attack's category (`BtlAttackGetCategory`) is 0xc, a set `cancelled` flag or a
   clash (`BtlAttackCheckClash`) spawns `cancelEffect` at `pos`, ends it and returns 1.
   Frame 0: launches it. Pitch is pi/4 at speed 40, or with a target (`BtlBakuganGetTarget` of
   the owner) speed = horizontal (xz) distance x 0.0133333 clamped to 35..45 and pitch = asin(distance x 0.25 /
   speed^2) (radians). `vel` is pitched up by rotating it with the quaternion of
   axis (-vel.z, vel.y, vel.x) (not normalised) and that angle, then yawed about y by `side`
   clamped to +-0.1396 rad; the effect's `dir` gets `vel`, `vel` is scaled by the speed,
   the effect's vtable slot 2 is called twice and `paramF1` (the step scale) is set to 1; returns 0.
   Later frames: with a target, the aim point is the target offset sideways by `side` (the
   horizontal attack-to-target vector turned 90 degrees and normalised); the yaw from `vel` to the
   aim point, wrapped to (-pi, pi], is turned 0.1 of the way (at most 0.0873 rad, scaled by `turn`)
   when it exceeds 0.0873 rad. Then `vel.y` drops by 0.5 (gravity), the effect's `dir` becomes
   the normalised `vel`, and `BtlAttackSweepHit` (hit kind `hitKind`, flags 3, mask 0x31bf337e)
   runs with `g_btlAttackHitCollider` cleared. On a hit the hit point is raised by 20 and
   `hitEffect` (0x25/0x9b +1 when `SaveGetProfileFlag0` is set) is spawned there with the owner
   and its id recorded; it ends and returns 1. Otherwise `pos` advances by `vel` x `paramF1` and it
   returns 0. */

u32 BtlAttackUpdateGuided(float turn, float side, BtlAttack *self, s32 hitKind, s32 hitEffect, s32 cancelEffect)
{
    float axis[4];
    float quat[4];
    float vec[4];
    float tmp[4];
    float offset[4];
    float aim[4];
    BtlBakugan *target;
    BtlBakugan *owner;
    GfxEffect *effect;
    GfxEffect *hitFx;
    const VtblEntry *entry;
    float pitch;
    float speed;
    float dist;
    float yaw;
    float heading;
    float want;
    float diff;
    float half;
    float s;
    float c;
    float x;
    float z;
    float len2;
    float inv;
    float k;

    if (!(self->age < 0x79)) {
        BtlAttackEnd(self);
        return 1;
    }
    if (BtlAttackGetCategory(self) != 0xc) {
        if (self->cancelled != 0 || BtlAttackCheckClash(self) != 0) {
            GfxEffectSpawn(g_btlAttackEffectMgr, cancelEffect, self->pos);
            BtlAttackEnd(self);
            return 1;
        }
    }

    if (self->age == 0) {
        pitch = 0.785398185f;
        speed = 40.0f;
        target = (BtlBakugan *)BtlBakuganGetTarget(self->owner);
        if (target != NULL) {
            /* dist = horizontal |pos - target pos| (y zeroed by vzero.s S011) */
            x = self->pos[0] - target->base.pos[0];
            z = self->pos[2] - target->base.pos[2];
            dist = __builtin_sqrtf(x * x + 0.0f + z * z);
            speed = dist * 0.0133333001f;
            if (speed < 35.0f) {
                speed = 35.0f;
            } else if (!(speed <= 45.0f)) {
                speed = 45.0f;
            }
            /* vasin.s (quarter turns) x pi/2 */
            pitch = __builtin_asinf(dist * -0.5f * -0.5f / (speed * speed));
        }
        /* axis = (-vel.z, vel.y, vel.x); quat = (axis x sin, cos) of pitch x 1/pi quarter turns
           (pitch / 2 radians); vel = quat x (vel.xyz, 0) x conj(quat) */
        axis[0] = -self->vel[2];
        axis[1] = self->vel[1];
        axis[2] = self->vel[0];
        half = 0.318309873f * pitch;
        c = VfCosQuarter(half);
        s = VfSinQuarter(half);
        quat[0] = axis[0] * s;
        quat[1] = axis[1] * s;
        quat[2] = axis[2] * s;
        quat[3] = c;
        vec[0] = self->vel[0];
        vec[1] = self->vel[1];
        vec[2] = self->vel[2];
        vec[3] = 0.0f;
        /* tmp = quat (x) vec */
        tmp[0] = quat[0] * vec[3] + quat[1] * vec[2] - quat[2] * vec[1] + quat[3] * vec[0];
        tmp[1] = -quat[0] * vec[2] + quat[1] * vec[3] + quat[2] * vec[0] + quat[3] * vec[1];
        tmp[2] = quat[0] * vec[1] - quat[1] * vec[0] + quat[2] * vec[3] + quat[3] * vec[2];
        tmp[3] = -quat[0] * vec[0] - quat[1] * vec[1] - quat[2] * vec[2] + quat[3] * vec[3];
        /* vel = tmp (x) conj(quat) */
        self->vel[0] = tmp[0] * quat[3] + tmp[1] * -quat[2] - tmp[2] * -quat[1] + tmp[3] * -quat[0];
        self->vel[1] = -tmp[0] * -quat[2] + tmp[1] * quat[3] + tmp[2] * -quat[0] + tmp[3] * -quat[1];
        self->vel[2] = tmp[0] * -quat[1] - tmp[1] * -quat[0] + tmp[2] * quat[3] + tmp[3] * -quat[2];
        self->vel[3] = -tmp[0] * -quat[0] - tmp[1] * -quat[1] - tmp[2] * -quat[2] + tmp[3] * quat[3];
        if (!(side <= 0.139626339f)) {
            side = 0.139626339f;
        } else if (side < -0.139626339f) {
            side = -0.139626339f;
        }
        /* vel.x/z rotated about y by side (vrot of side x 2/pi quarter turns) */
        c = __builtin_cosf(side);
        s = __builtin_sinf(side);
        x = self->vel[0];
        z = self->vel[2];
        self->vel[0] = x * c - z * s;
        self->vel[2] = x * s + z * c;
        /* effect dir = vel; vel.xyz *= speed, w = 0 */
        effect = (GfxEffect *)self->effect;
        effect->dir[0] = self->vel[0];
        effect->dir[1] = self->vel[1];
        effect->dir[2] = self->vel[2];
        effect->dir[3] = self->vel[3];
        self->vel[0] = self->vel[0] * speed;
        self->vel[1] = self->vel[1] * speed;
        self->vel[2] = self->vel[2] * speed;
        self->vel[3] = 0.0f;
        effect = (GfxEffect *)self->effect;
        entry = &((const VtblEntry *)effect->base.vtable)[2];
        ((void (*)(void *))entry->fn)((u8 *)effect + entry->delta);
        effect = (GfxEffect *)self->effect;
        entry = &((const VtblEntry *)effect->base.vtable)[2];
        ((void (*)(void *))entry->fn)((u8 *)effect + entry->delta);
        self->paramF1 = 1.0f;
        return 0;
    }

    target = (BtlBakugan *)BtlBakuganGetTarget(self->owner);
    if (target != NULL) {
        heading = atan2f(self->vel[2], self->vel[0]);
        /* offset = target pos - pos (xyz), y = 0, turned 90 degrees: (-z, 0, x) */
        offset[0] = target->base.pos[0] - self->pos[0];
        offset[2] = target->base.pos[2] - self->pos[2];
        offset[1] = 0.0f;
        x = -offset[2];
        z = offset[0];
        /* offset = normalise(x, 0, z) x side (zero length: scale 0) */
        len2 = x * x + offset[1] * offset[1] + z * z;
        if (len2 == 0.0f) {
            inv = 0.0f;
        } else {
            inv = VfRsq(len2);
        }
        k = inv * side;
        offset[0] = x * k;
        offset[1] = offset[1] * k;
        offset[2] = z * k;
        aim[0] = target->base.pos[0] + offset[0];
        aim[2] = target->base.pos[2] + offset[2];
        want = atan2f(aim[2] - self->pos[2], aim[0] - self->pos[0]);
        if (!(want <= 3.14159274f)) {
            want = want - 6.28318548f;
        } else if (want <= -3.14159274f) {
            want = want + 6.28318548f;
        }
        diff = heading - want;
        diff = diff - (float)(s32)(diff * 0.318309873f) * 6.28318548f;
        if (diff < 0.0f) {
            diff = diff + 6.28318548f;
        }
        if (diff < 3.14159274f) {
            diff = -diff;
        } else {
            diff = 6.28318548f - diff;
        }
        if (!(__builtin_fabsf(diff) <= 0.0872664601f)) {
            yaw = diff * 0.100000001f;
            if (!(yaw <= 0.0872664601f)) {
                yaw = 0.0872664601f;
            } else if (yaw < -0.0872664601f) {
                yaw = -0.0872664601f;
            }
            yaw = yaw * turn;
            /* vel.x/z rotated about y by yaw (vrot of yaw x 2/pi quarter turns) */
            c = __builtin_cosf(yaw);
            s = __builtin_sinf(yaw);
            x = self->vel[0];
            z = self->vel[2];
            self->vel[0] = x * c - z * s;
            self->vel[2] = x * s + z * c;
        }
    }

    self->vel[1] = self->vel[1] + -0.5f;
    /* effect dir = vel */
    effect = (GfxEffect *)self->effect;
    effect->dir[0] = self->vel[0];
    effect->dir[1] = self->vel[1];
    effect->dir[2] = self->vel[2];
    effect->dir[3] = self->vel[3];
    /* effect dir.xyz = normalise(dir.xyz) clamped to [-1, 1] (zero length: scale 0); w = 0 */
    effect = (GfxEffect *)self->effect;
    len2 = effect->dir[0] * effect->dir[0] + effect->dir[1] * effect->dir[1] + effect->dir[2] * effect->dir[2];
    if (len2 == 0.0f) {
        inv = 0.0f;
    } else {
        inv = VfRsq(len2);
    }
    effect->dir[0] = VfSat1(effect->dir[0] * inv);
    effect->dir[1] = VfSat1(effect->dir[1] * inv);
    effect->dir[2] = VfSat1(effect->dir[2] * inv);
    effect->dir[3] = 0.0f;
    g_btlAttackHitCollider = NULL;
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, hitKind, 3, 0, 0x31bf337e) != 0) {
        g_btlAttackHitPoint.y = g_btlAttackHitPoint.y + 20.0f;
        if ((hitEffect == 0x25 || hitEffect == 0x9b) && SaveGetProfileFlag0() != 0) {
            hitEffect = hitEffect + 1;
        }
        hitFx = (GfxEffect *)GfxEffectSpawn(g_btlAttackEffectMgr, hitEffect, &g_btlAttackHitPoint.x);
        owner = self->owner;
        hitFx->ownerBakugan = owner;
        if (owner != NULL) {
            hitFx->ownerId = owner->base.base.id;
        }
        BtlAttackEnd(self);
        return 1;
    }
    /* pos.xyz += vel.xyz x paramF1 */
    k = self->paramF1;
    self->pos[0] = self->pos[0] + self->vel[0] * k;
    self->pos[1] = self->pos[1] + self->vel[1] * k;
    self->pos[2] = self->pos[2] + self->vel[2] * k;
    return 0;
}
