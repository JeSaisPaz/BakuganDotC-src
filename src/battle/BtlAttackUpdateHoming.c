// bdc 0x0887b004 BtlAttackUpdateHoming
#include "bdc.h"

/* Shared body of the curving homing projectile types (`BtlAttackType04Update`,
   `BtlAttackType43Update`, `BtlAttackType77Update`). After 60 frames it ends
   (`BtlAttackEnd`). The target is the owner's target (`BtlBakuganGetTarget`).
   Frame 0: stores `speed` in `mtx[0][0]`, scales `vel` by it, picks the curve side from the
   owner's input heading relative to its facing (turning left curves by +0.5654867 rad, right by
   -0.5654867, x1.8 when `paramF2` was non-zero) into `paramF0`, the pitch curve 0.1570796 (same
   factor) into `paramF1`, sets the curve weight `paramF2` = 1, the best distance `mtx[0][1]` =
   infinity, and with a target copies its position into the chase point `auxVec`.
   With an attached effect, every frame: without a target or once released (`param0`) it homes
   (`BtlAttackSteerToTarget` speed `mtx[0][0]`, height 110) and stays released. Otherwise the
   chase point moves 0.3 of the way toward the target (at most 20 units); when the squared
   distance from the attack to the chase point raised by 100 grows past the best so far or drops
   below 40000 (200 units) it releases: `dir` = normalised `vel`, homing with height 50 and turn
   rate 0.08. Else it records the new best, eases the curve weight toward (distance to the point /
   owner's distance to the chase point) by 0.2 (not below 0), and aims `vel` at the point's yaw
   and pitch (`atan2f`) plus `paramF0`/`paramF1` x weight, times the speed. With a target it then
   builds the effect's matrix: forward = normalised `vel`, aimed through a point 450 units to the
   side of the owner-target midpoint (the owner-to-target vector rotated 90 degrees about y by a
   quaternion), with identity translation. From frame 1, unless `BtlAttackResolveClash`
   (impact `hitId`) consumed it, it sweeps along `vel` (`BtlAttackSweepHit` hit kind
   `hitEffect`) and on a hit queues impact `hitId` at the hit point (`BtlAttackSetPendingHit`;
   the binary tests the hit collider but calls it the same way on both paths); otherwise the
   position advances by `vel`. */

void BtlAttackUpdateHoming(BtlAttack *self, s32 hitId, s32 hitEffect, float speed)
{
    float chase[4];
    float delta[4];
    float axis[4];
    float quat[4];
    float conj[4];
    float vq[4];
    float tmp[4];
    float rotated[4];
    float half[4];
    float rel[4];
    float fwd[3];
    float right[3];
    float up[3];
    float *m;
    BtlBakugan *target;
    float scale;
    float diff;
    float lenSq;
    float inv;
    float distSq;
    float ownerDist;
    float weight;
    float yaw;
    float pitch;
    float cosPitch;
    float cosYaw;
    float sinPitch;
    float sinYaw;
    float halfTurn;
    float s;
    float c;
    float ox;
    float oy;
    float oz;

    if (self->age > 60) {
        BtlAttackEnd(self);
        return;
    }
    target = (BtlBakugan *)BtlBakuganGetTarget(self->owner);
    if (self->age == 0) {
        self->mtx[0][0] = speed;
        self->vel[0] = self->vel[0] * speed;
        self->vel[1] = self->vel[1] * speed;
        self->vel[2] = self->vel[2] * speed;
        self->vel[3] = 0.0f; /* lane 3 of C710 = S713 */
        scale = 1.0f;
        if (!(self->paramF2 == 0.0f)) {
            scale = 1.79999995f;
        }
        diff = self->owner->input->heading - self->owner->base.rot[1];
        diff = diff - (float)(s32)(diff * 0.318309873f) * 6.28318548f;
        if (diff < 0.0f) {
            diff = diff + 6.28318548f;
        }
        if (diff < 3.14159274f) {
            diff = -diff;
        } else {
            diff = 6.28318548f - diff;
        }
        if (diff <= 0.0f) {
            self->paramF0 = scale * 0.565486729f;
        } else {
            self->paramF0 = scale * -0.565486729f;
        }
        self->paramF2 = 1.0f;
        self->paramF1 = scale * 0.157079637f;
        self->mtx[0][1] = INFINITY;
        if (target != NULL) {
            self->auxVec[0] = target->base.pos[0];
            self->auxVec[1] = target->base.pos[1];
            self->auxVec[2] = target->base.pos[2];
            self->auxVec[3] = target->base.pos[3];
        }
    }

    if (self->effect != NULL) {
        if (target == NULL || self->param0 != 0) {
            BtlAttackSteerToTarget(self->mtx[0][0], 110.0f, self, 1, NULL);
            self->param0 = 1;
        } else {
            /* delta = target - auxVec (xyz, w = target w); chase = 0.3 x delta (w = 0) */
            delta[0] = target->base.pos[0] - self->auxVec[0];
            delta[1] = target->base.pos[1] - self->auxVec[1];
            delta[2] = target->base.pos[2] - self->auxVec[2];
            delta[3] = target->base.pos[3];
            chase[0] = delta[0] * 0.300000012f;
            chase[1] = delta[1] * 0.300000012f;
            chase[2] = delta[2] * 0.300000012f;
            chase[3] = 0.0f;
            lenSq = chase[0] * chase[0] + chase[1] * chase[1] + chase[2] * chase[2];
            if (!(lenSq <= 400.0f)) {
                /* clamp the step to 20 units */
                lenSq = chase[0] * chase[0] + chase[1] * chase[1] + chase[2] * chase[2];
                if (lenSq == 0.0f) {
                    inv = 0.0f;
                } else {
                    inv = VfRsq(lenSq);
                }
                inv = inv * 20.0f;
                chase[0] = chase[0] * inv;
                chase[1] = chase[1] * inv;
                chase[2] = chase[2] * inv;
                chase[3] = 0.0f;
            }
            self->auxVec[0] = self->auxVec[0] + chase[0];
            self->auxVec[1] = self->auxVec[1] + chase[1];
            self->auxVec[2] = self->auxVec[2] + chase[2];
            chase[0] = self->auxVec[0];
            chase[1] = self->auxVec[1];
            chase[2] = self->auxVec[2];
            chase[3] = self->auxVec[3];
            chase[1] = chase[1] + 100.0f;
            chase[0] = chase[0] - self->pos[0];
            chase[1] = chase[1] - self->pos[1];
            chase[2] = chase[2] - self->pos[2];
            distSq = chase[0] * chase[0] + chase[1] * chase[1] + chase[2] * chase[2];
            if (self->mtx[0][1] < distSq || distSq < 40000.0f) {
                self->param0 = 1;
                /* dir.xyz = normalise(vel.xyz), clamped to [-1, 1]; w = S713 */
                lenSq = self->vel[0] * self->vel[0] + self->vel[1] * self->vel[1] +
                        self->vel[2] * self->vel[2];
                if (lenSq == 0.0f) {
                    inv = 0.0f;
                } else {
                    inv = VfRsq(lenSq);
                }
                self->dir[0] = VfSat1(self->vel[0] * inv);
                self->dir[1] = VfSat1(self->vel[1] * inv);
                self->dir[2] = VfSat1(self->vel[2] * inv);
                self->dir[3] = 0.0f;
                BtlAttackSteerToTarget(self->mtx[0][0], 50.0f, self, 1, NULL);
                self->turnRate = 0.0799999982f;
            } else {
                self->mtx[0][1] = distSq;
                ox = self->owner->base.pos[0] - self->auxVec[0];
                oy = self->owner->base.pos[1] - self->auxVec[1];
                oz = self->owner->base.pos[2] - self->auxVec[2];
                ownerDist = __builtin_sqrtf(ox * ox + oy * oy + oz * oz);
                weight = self->paramF2 +
                         (__builtin_sqrtf(distSq) / ownerDist - self->paramF2) * 0.200000003f;
                self->paramF2 = weight;
                if (weight < 0.0f) {
                    self->paramF2 = 0.0f;
                }
                yaw = atan2f(chase[2], chase[0]);
                yaw = yaw + self->paramF0 * self->paramF2;
                pitch = atan2f(chase[1],
                               __builtin_sqrtf(chase[0] * chase[0] + chase[2] * chase[2]));
                pitch = pitch + self->paramF1 * self->paramF2;
                /* vcos/vsin of angle x S703 (2/pi): plain cos/sin of the angle */
                cosPitch = __builtin_cosf(pitch);
                cosYaw = __builtin_cosf(yaw);
                sinPitch = __builtin_sinf(pitch);
                sinYaw = __builtin_sinf(yaw);
                self->vel[0] = cosYaw * cosPitch;
                self->vel[1] = sinPitch;
                self->vel[2] = sinYaw * cosPitch;
                self->vel[3] = 0.0f;
                self->vel[0] = self->vel[0] * self->mtx[0][0];
                self->vel[1] = self->vel[1] * self->mtx[0][0];
                self->vel[2] = self->vel[2] * self->mtx[0][0];
                self->vel[3] = 0.0f;
            }
        }

        if (target != NULL) {
            /* rel = target - owner (xyz, w = target w) */
            rel[0] = target->base.pos[0] - self->owner->base.pos[0];
            rel[1] = target->base.pos[1] - self->owner->base.pos[1];
            rel[2] = target->base.pos[2] - self->owner->base.pos[2];
            rel[3] = target->base.pos[3];
            /* horizontal perpendicular axis (-rel.z, 0, rel.x), normalised and clamped */
            axis[0] = -rel[2];
            axis[1] = 0.0f;
            axis[2] = rel[0];
            lenSq = axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2];
            if (lenSq == 0.0f) {
                inv = 0.0f;
            } else {
                inv = VfRsq(lenSq);
            }
            axis[0] = VfSat1(axis[0] * inv);
            axis[1] = VfSat1(axis[1] * inv);
            axis[2] = VfSat1(axis[2] * inv);
            axis[3] = 0.0f;
            /* quat = (axis x sin, cos) of half of pi/2: the VFPU angle is 1/pi x pi/2 quarter
               turns */
            halfTurn = 0.318309873f * 1.57079637f;
            c = VfCosQuarter(halfTurn);
            s = VfSinQuarter(halfTurn);
            quat[0] = axis[0] * s;
            quat[1] = axis[1] * s;
            quat[2] = axis[2] * s;
            quat[3] = c;
            conj[0] = -quat[0];
            conj[1] = -quat[1];
            conj[2] = -quat[2];
            conj[3] = quat[3];
            vq[0] = rel[0];
            vq[1] = rel[1];
            vq[2] = rel[2];
            vq[3] = 0.0f; /* S203 = S730 */
            /* tmp = quat x vq; rotated = tmp x conj */
            tmp[0] = quat[0] * vq[3] + quat[1] * vq[2] - quat[2] * vq[1] + quat[3] * vq[0];
            tmp[1] = -quat[0] * vq[2] + quat[1] * vq[3] + quat[2] * vq[0] + quat[3] * vq[1];
            tmp[2] = quat[0] * vq[1] - quat[1] * vq[0] + quat[2] * vq[3] + quat[3] * vq[2];
            tmp[3] = -quat[0] * vq[0] - quat[1] * vq[1] - quat[2] * vq[2] + quat[3] * vq[3];
            rotated[0] = tmp[0] * conj[3] + tmp[1] * conj[2] - tmp[2] * conj[1] + tmp[3] * conj[0];
            rotated[1] = -tmp[0] * conj[2] + tmp[1] * conj[3] + tmp[2] * conj[0] + tmp[3] * conj[1];
            rotated[2] = tmp[0] * conj[1] - tmp[1] * conj[0] + tmp[2] * conj[3] + tmp[3] * conj[2];
            rotated[3] = -tmp[0] * conj[0] - tmp[1] * conj[1] - tmp[2] * conj[2] + tmp[3] * conj[3];
            /* chase = rotated scaled to 450 units (w = 0) */
            lenSq = rotated[0] * rotated[0] + rotated[1] * rotated[1] + rotated[2] * rotated[2];
            if (lenSq == 0.0f) {
                inv = 0.0f;
            } else {
                inv = VfRsq(lenSq);
            }
            inv = inv * 450.0f;
            chase[0] = rotated[0] * inv;
            chase[1] = rotated[1] * inv;
            chase[2] = rotated[2] * inv;
            chase[3] = 0.0f;
            /* aim = chase + owner + 0.5 x rel - pos */
            half[0] = rel[0] * 0.5f;
            half[1] = rel[1] * 0.5f;
            half[2] = rel[2] * 0.5f;
            half[3] = 0.0f;
            rotated[0] = self->owner->base.pos[0] + half[0];
            rotated[1] = self->owner->base.pos[1] + half[1];
            rotated[2] = self->owner->base.pos[2] + half[2];
            rotated[3] = self->owner->base.pos[3];
            chase[0] = chase[0] + rotated[0];
            chase[1] = chase[1] + rotated[1];
            chase[2] = chase[2] + rotated[2];
            chase[0] = chase[0] - self->pos[0];
            chase[1] = chase[1] - self->pos[1];
            chase[2] = chase[2] - self->pos[2];
            /* fwd = normalise(vel), clamped */
            lenSq = self->vel[0] * self->vel[0] + self->vel[1] * self->vel[1] +
                    self->vel[2] * self->vel[2];
            if (lenSq == 0.0f) {
                inv = 0.0f;
            } else {
                inv = VfRsq(lenSq);
            }
            fwd[0] = VfSat1(self->vel[0] * inv);
            fwd[1] = VfSat1(self->vel[1] * inv);
            fwd[2] = VfSat1(self->vel[2] * inv);
            /* right = normalise(aim x fwd), clamped */
            right[0] = chase[1] * fwd[2] - chase[2] * fwd[1];
            right[1] = chase[2] * fwd[0] - chase[0] * fwd[2];
            right[2] = chase[0] * fwd[1] - chase[1] * fwd[0];
            lenSq = right[0] * right[0] + right[1] * right[1] + right[2] * right[2];
            if (lenSq == 0.0f) {
                inv = 0.0f;
            } else {
                inv = VfRsq(lenSq);
            }
            right[0] = VfSat1(right[0] * inv);
            right[1] = VfSat1(right[1] * inv);
            right[2] = VfSat1(right[2] * inv);
            /* up = fwd x right */
            up[0] = fwd[1] * right[2] - fwd[2] * right[1];
            up[1] = fwd[2] * right[0] - fwd[0] * right[2];
            up[2] = fwd[0] * right[1] - fwd[1] * right[0];
            /* rows right, up, fwd with w = 0, then (0, 0, 0, 1) */
            m = ((GfxEffect *)self->effect)->matrix;
            m[0] = right[0];
            m[1] = right[1];
            m[2] = right[2];
            m[3] = 0.0f;
            m[4] = up[0];
            m[5] = up[1];
            m[6] = up[2];
            m[7] = 0.0f;
            m[8] = fwd[0];
            m[9] = fwd[1];
            m[10] = fwd[2];
            m[11] = 0.0f;
            m[12] = 0.0f;
            m[13] = 0.0f;
            m[14] = 0.0f;
            m[15] = 1.0f;
        }
    }

    if (self->age != 0) {
        if (BtlAttackResolveClash(self, hitId) != 0) {
            return;
        }
        if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, hitEffect, 3, 0,
                              0x31bf337e) != 0) {
            BtlAttackSetPendingHit(self, hitId, &g_btlAttackHitPoint.x);
            return;
        }
    }
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
}
