// bdc 0x08860ae4 BtlBakuganMoveCollide
#include "bdc.h"

/* Moves the Bakugan `self` by the displacement `delta` (vec4; `delta.y` is rewritten) with stage
   collision. Returns 1 when the move landed the unit or put it back (each such case clears
   stateFlags bit 0x40000000), else 0.
   - move = delta + tiltPush with y cleared; delta.y += tiltPush.y; tiltPush = 0.
     Near the ground (`pos.y < groundY + 20`) the move is rotated by the quaternion taking +Y onto
     `groundNormal` (identity when the normal is vertical); a negative rotated y is eased into
     `slopeDescent` (step 0.1) and used, otherwise `slopeDescent` = 0. pos.xyz += move.xyz.
   - Horizontal move (move.x or move.z != 0): the sphere query `g_btlBakuganSphereQuery`
     (radius = `radius`, centre = pos raised by the radius, vtable entry 9 = update) is cast with
     `CollisionRaycast`. On a hit: layer 8 sets stateFlags 0x200; a surface-0xb hit facing the
     unit (dot of (cos, 0, sin) of `rot.y` with the normal < -0.7) is stored in
     `wallHitPoint`/`wallHitNormal` with stateFlags 0x20000 (checked again on the later hits). A
     sphere swept from the start position along the move: when it hits, a segment from 5 (13 in
     water stages 0xc..0xf, `BtlBakuganIsInWaterStage0CTo0F`) above the sweep start toward the
     hit, lengthened by the radius, tells a step (no segment hit: restore the first hit and
     `BtlBakuganResolveSphereHit`) from a wall (segment hit: move to the sweep fraction - 0.01
     and slide along the hit normal's tangent at 0.8, y = 0). Then a sphere cast resolves a
     remaining hit, and if a second cast still hits the unit goes back to the start position.
     `collisionPushback` = intended - resolved position; a rise from the start is taken off
     delta.y.
   - Rising (`delta.y > 0`, or NaN): a sphere of r' = radius - 10 swept up by delta.y + `height`
     from pos; a ceiling hit above `pos.y + 1.2 * r'` within 0.5 * r' of the probe axis sets
     delta.y to the clearance. If the ceiling point is horizontally within 0.4 * r' of pos, or an
     8-unit upward segment from beside it hits, the y velocity is cleared; otherwise 0.2 of the
     horizontal direction away from the ceiling point is added to `tiltPush`. Then
     pos.y += delta.y and delta.y = -0.1.
   - Falling (delta.y <= 0): with stateFlags bit 8 pos.y += delta.y, clamped up to `groundY`;
     otherwise a downward sweep sets `contactObj` and snaps to the floor, lands per the slope /
     airborne frames / fall speed rules (flags 0x2000 is set when airborne >= 26 frames and
     velocity.y < -20) and, on a slope (normal.y <= 0.78) it did not land on, pushes `tiltPush`
     downhill by (dot(dir(delta), downhill) - 0.4) * 24 (>= 0); with the low byte of `flags`
     set and dir(tiltPush) . dir(velocity) not negative, the horizontal velocity is taken off
     `tiltPush` and velocity.x/z are scaled by 0.9.
   - Finally a moved unit that hit a wall while falling (player, or stateFlags & 0x4040010) sweeps
     a half-radius sphere along the whole move and is put back at the start on a hit.
   VFPU bank constants written as literals: C720 / S713 = 0, C730 = (0, 0, 0, 1), S703 = 2/pi. */

/* 1 / |v.xyz|, 0 for a zero-length v (vcmp EZ + vcmovt from the bank zero S713). */
static float MoveCollideInvLength(const float *v)
{
    float lenSq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    float inv = VfRsq(lenSq);

    if (lenSq == 0.0f) {
        inv = 0.0f;
    }
    return inv;
}

static float MoveCollideLength(const float *v)
{
    return __builtin_sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

static float MoveCollideDot(const float *a, const float *b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static void MoveCollideCopy(float *dst, const float *src)
{
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}

/* dst.xyz = a.xyz + b.xyz, dst.w = a.w (vadd.t on a loaded quad). */
static void MoveCollideAdd(float *dst, const float *a, const float *b)
{
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
    dst[3] = a[3];
}

/* dst.xyz = a.xyz - b.xyz, dst.w = a.w (vsub.t on a loaded quad). */
static void MoveCollideSub(float *dst, const float *a, const float *b)
{
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
    dst[2] = a[2] - b[2];
    dst[3] = a[3];
}

/* dst.xyz = v.xyz * k, dst.w = 0 (C710 result, w = bank S713). */
static void MoveCollideScale(float *dst, const float *v, float k)
{
    dst[0] = v[0] * k;
    dst[1] = v[1] * k;
    dst[2] = v[2] * k;
    dst[3] = 0.0f;
}

/* dst.xyz = v.xyz / |v| clamped to [-1, 1] (vpfxd [-1:1,-1:1,-1:1,M]), dst.w = 0. */
static void MoveCollideNormalise(float *dst, const float *v)
{
    float inv = MoveCollideInvLength(v);

    dst[0] = VfSat1(v[0] * inv);
    dst[1] = VfSat1(v[1] * inv);
    dst[2] = VfSat1(v[2] * inv);
    dst[3] = 0.0f;
}

/* dst.xyz = v.xyz * (len / |v|) (no clamp), dst.w = 0. */
static void MoveCollideResize(float *dst, const float *v, float len)
{
    MoveCollideScale(dst, v, MoveCollideInvLength(v) * len);
}

/* d = s * t (vqmul.q, w last). */
static void MoveCollideQuatMul(float *d, const float *s, const float *t)
{
    float x = s[0] * t[3] + s[1] * t[2] - s[2] * t[1] + s[3] * t[0];
    float y = -s[0] * t[2] + s[1] * t[3] + s[2] * t[0] + s[3] * t[1];
    float z = s[0] * t[1] - s[1] * t[0] + s[2] * t[3] + s[3] * t[2];
    float w = -s[0] * t[0] - s[1] * t[1] - s[2] * t[2] + s[3] * t[3];

    d[0] = x;
    d[1] = y;
    d[2] = z;
    d[3] = w;
}

/* dst = q * (v.xyz, 0) * conj(q), all four lanes. */
static void MoveCollideQuatRotate(float *dst, const float *q, const float *v)
{
    float conj[4];
    float vec[4];
    float tmp[4];

    conj[0] = -q[0];
    conj[1] = -q[1];
    conj[2] = -q[2];
    conj[3] = q[3];
    vec[0] = v[0];
    vec[1] = v[1];
    vec[2] = v[2];
    vec[3] = 0.0f; /* S730 */
    MoveCollideQuatMul(tmp, q, vec);
    MoveCollideQuatMul(dst, tmp, conj);
}

/* quat = (axis.xyz * sin(angle / 2), cos(angle / 2)): vcst 1/PI times the angle, in quarter turns. */
static void MoveCollideAxisAngleQuat(float *quat, const float *axis, float angle)
{
    float a = 0.318309873f * angle;
    float c = VfCosQuarter(a);
    float s = VfSinQuarter(a);

    quat[0] = axis[0] * s;
    quat[1] = axis[1] * s;
    quat[2] = axis[2] * s;
    quat[3] = c;
}

/* A surface-0xb hit whose normal faces the unit's heading is kept as the wall hit. */
static void MoveCollideNoteWallHit(BtlBakugan *self)
{
    const float *normal = (const float *)&g_collisionHitResult.normal;
    float yaw;
    float facing;

    if (g_collisionHitInfo.surface == 0xb) {
        /* vrot.q [C,0,S,0] of rot.y * S703 */
        yaw = self->base.rot[1];
        facing = __builtin_cosf(yaw) * normal[0] + 0.0f * normal[1] + __builtin_sinf(yaw) * normal[2];
        if (facing < -0.699999988f) {
            MoveCollideCopy(self->wallHitPoint, (const float *)&g_collisionHitResult.point);
            MoveCollideCopy(self->wallHitNormal, normal);
            self->stateFlags = self->stateFlags | 0x20000;
        }
    }
}

/* Centre the sphere query on `pos` raised by its radius and call its vtable entry 9 (update). */
static void MoveCollideUpdateSphere(CollisionSphereQuery *sphere, const float *pos)
{
    const VtblEntry *update;

    MoveCollideCopy((float *)&sphere->center, pos);
    sphere->center.y = sphere->center.y + sphere->radius;
    update = &sphere->vtbl[9];
    ((void (*)(void *))update->fn)((u8 *)sphere + update->delta);
}

char BtlBakuganMoveCollide(BtlBakugan *self, float *delta, u32 flags)
{
    CollisionSphereQuery *sphere = &g_btlBakuganSphereQuery;
    CollisionSweptSphereDesc *swept = &g_collisionSweptSphereDesc;
    SegmentShape *segment = &g_collisionSegmentDesc;
    float *sweptStart = (float *)&swept->start;
    float *sweptDir = (float *)&swept->dir;
    float *hitPoint = (float *)&g_collisionHitResult.point;
    float *hitNormal = (float *)&g_collisionHitResult.normal;
    float *pos = self->base.pos;
    float move[4];       /* rotated / resolved move */
    float startPos[4];   /* position on entry */
    float quat[4];       /* +Y -> groundNormal */
    float axis[4];
    float savedPoint[4]; /* first sphere hit */
    float savedNormal[4];
    float stepNormal[4]; /* swept-sphere hit normal */
    float toHit[4];
    float stepBase[4];
    float tangent[4];
    float tmp[4];
    float probeTop[4];
    float probeEnd[4];
    float push[4];
    float slide[4];
    float slideQuat[4];
    float moveDir[4];
    float pushDir[4];
    float velDir[4];
    CollisionCollider *hit;
    CollisionCollider *floor;
    float bestT;
    float value;
    u8 limitPush;
    u8 result;
    u8 hitWall;
    u8 resolved;
    u8 falling;
    bool alongY;

    limitPush = (u8)flags;
    result = 0;
    MoveCollideCopy(move, delta);
    MoveCollideCopy(startPos, pos);
    MoveCollideAdd(move, move, self->tiltPush);
    delta[1] = delta[1] + self->tiltPush[1];
    move[1] = 0.0f;
    /* C720: bank zero vector */
    self->tiltPush[0] = 0.0f;
    self->tiltPush[1] = 0.0f;
    self->tiltPush[2] = 0.0f;
    self->tiltPush[3] = 0.0f;

    if (pos[1] < self->groundY + 20.0f) {
        if (self->groundNormal[0] * self->groundNormal[0] +
                self->groundNormal[2] * self->groundNormal[2] <
            9.99999975e-06f) {
            alongY = !(self->groundNormal[1] * self->groundNormal[1] <= 9.99999975e-05f);
        } else {
            alongY = false;
        }
        if (alongY) {
            /* C730: bank identity quaternion */
            quat[0] = 0.0f;
            quat[1] = 0.0f;
            quat[2] = 0.0f;
            quat[3] = 1.0f;
        } else {
            /* axis.w is never set (stale stack lane) and never read */
            axis[0] = self->groundNormal[2];
            axis[1] = 0.0f;
            axis[2] = -self->groundNormal[0];
            MoveCollideNormalise(axis, axis);
            MoveCollideAxisAngleQuat(quat, axis, acosf(self->groundNormal[1]));
        }
        MoveCollideQuatRotate(move, quat, move);
        if (move[1] < 0.0f) {
            float descent = self->slopeDescent;

            descent = descent + (move[1] - descent) * 0.100000001f;
            self->slopeDescent = descent;
            move[1] = descent;
        } else {
            self->slopeDescent = 0.0f;
        }
    }

    MoveCollideAdd(pos, pos, move);
    MoveCollideCopy(sweptDir, move);
    sphere->radius = self->radius;
    hitWall = 0;
    resolved = 0;
    /* C720: bank zero vector */
    self->collisionPushback[0] = 0.0f;
    self->collisionPushback[1] = 0.0f;
    self->collisionPushback[2] = 0.0f;
    self->collisionPushback[3] = 0.0f;

    if (move[0] != 0.0f || move[2] != 0.0f) {
        MoveCollideCopy(self->collisionPushback, pos);
        MoveCollideUpdateSphere(sphere, pos);
        hit = CollisionRaycast(self->collisionMask, &g_collisionSphereBlock, 1);
        if (hit != NULL) {
            if (hit->layer == 8) {
                self->stateFlags = self->stateFlags | 0x200;
            }
            hitWall = 1;
            MoveCollideNoteWallHit(self);
            MoveCollideCopy(savedPoint, hitPoint);
            MoveCollideCopy(savedNormal, hitNormal);
            MoveCollideCopy(sweptStart, startPos);
            swept->start.y = swept->start.y + sphere->radius;
            swept->radius = sphere->radius;
            swept->start.w = sphere->radius * sphere->radius;
            if (CollisionRaycast(self->collisionMask, swept->shapeBlock, 1) != NULL) {
                bestT = g_collisionHitInfo.bestT;
                MoveCollideCopy(stepNormal, hitNormal);
                MoveCollideCopy(segment->start, sweptStart);
                segment->start[1] = segment->start[1] - 5.0f;
                MoveCollideSub(toHit, hitPoint, sweptStart);
                toHit[1] = 0.0f;
                if (BtlBakuganIsInWaterStage0CTo0F(self) != 0) {
                    segment->start[1] = segment->start[1] + 18.0f;
                } else {
                    segment->start[1] = segment->start[1] + 10.0f;
                }
                /* segment->dir = toHit + normalize(toHit) * radius (w = the hit point's w) */
                MoveCollideCopy(segment->dir, toHit);
                MoveCollideResize(toHit, toHit, sphere->radius);
                MoveCollideAdd(segment->dir, segment->dir, toHit);
                if (CollisionRaycast(self->collisionMask, &g_collisionSegmentBlock, 0) == NULL) {
                    /* A step: resolve against the first sphere hit. */
                    MoveCollideCopy(hitPoint, savedPoint);
                    MoveCollideCopy(hitNormal, savedNormal);
                    BtlBakuganResolveSphereHit(self, sphere, pos, move, 0);
                    resolved = 1;
                } else {
                    /* A wall: stop short of it and slide along tangent (-n.z, n.y, n.x). */
                    MoveCollideScale(tmp, move, bestT - 0.00999999978f);
                    MoveCollideAdd(stepBase, startPos, tmp);
                    tangent[0] = -stepNormal[2];
                    tangent[1] = stepNormal[1];
                    tangent[2] = stepNormal[0];
                    tangent[3] = stepNormal[3];
                    MoveCollideScale(move, tangent, MoveCollideDot(move, tangent) * 0.800000012f);
                    move[1] = 0.0f;
                    MoveCollideAdd(pos, stepBase, move);
                    MoveCollideUpdateSphere(sphere, pos);
                }
            }

            MoveCollideUpdateSphere(sphere, pos);
            if (CollisionRaycast(self->collisionMask, &g_collisionSphereBlock, 1) != NULL) {
                MoveCollideNoteWallHit(self);
                BtlBakuganResolveSphereHit(self, sphere, pos, move, 0);
                MoveCollideUpdateSphere(sphere, pos);
                if (CollisionRaycast(self->collisionMask, &g_collisionSphereBlock, 1) != NULL) {
                    MoveCollideNoteWallHit(self);
                    /* Still stuck: back to the start position. */
                    MoveCollideCopy(pos, startPos);
                }
            }
        }
        MoveCollideSub(self->collisionPushback, self->collisionPushback, pos);
        value = pos[1] - startPos[1];
        if (!(value <= 0.0f)) {
            delta[1] = delta[1] - value;
        }
    }

    falling = delta[1] <= 0.0f;
    MoveCollideUpdateSphere(sphere, pos);

    if (!falling) {
        /* Rising: probe for a ceiling over the head. */
        swept->radius = self->radius - 10.0f;
        MoveCollideCopy(sweptStart, pos);
        swept->start.y = ((self->radius - 10.0f) + delta[1]) + swept->start.y;
        value = self->height + delta[1];
        swept->dir.x = 0.0f;
        swept->dir.y = value;
        swept->dir.z = 0.0f;
        swept->dir.w = 0.0f;
        if (CollisionRaycast(self->collisionMask, swept->shapeBlock, 4) != NULL &&
            pos[1] + swept->radius * 1.20000005f < g_collisionHitResult.point.y) {
            MoveCollideAdd(probeTop, sweptStart, sweptDir);
            MoveCollideSub(push, probeTop, hitPoint);
            push[1] = 0.0f;
            if (MoveCollideLength(push) < (self->radius - 10.0f) * 0.5f) {
                u8 pushOut;

                delta[1] = ((swept->start.y + g_collisionHitInfo.bestDist) -
                            ((self->radius - 10.0f) + self->height)) -
                           pos[1];
                pushOut = 1;
                MoveCollideSub(push, pos, hitPoint);
                push[1] = 0.0f;
                if (MoveCollideLength(push) < (self->radius - 10.0f) * 0.400000006f) {
                    pushOut = 0;
                    self->base.velocity[1] = 0.0f;
                } else {
                    /* An 8-unit vertical segment from 3 below the ceiling point, 0.5 outward. */
                    MoveCollideResize(push, push, 0.5f);
                    MoveCollideAdd(probeEnd, hitPoint, push);
                    MoveCollideCopy(segment->start, probeEnd);
                    segment->start[1] = segment->start[1] - 3.0f;
                    segment->dir[0] = g_vecUp.x * 8.0f;
                    segment->dir[1] = g_vecUp.y * 8.0f;
                    segment->dir[2] = g_vecUp.z * 8.0f;
                    segment->dir[3] = 0.0f;
                    if (CollisionRaycast(self->collisionMask, &g_collisionSegmentBlock, 0) !=
                        NULL) {
                        pushOut = 0;
                        self->base.velocity[1] = 0.0f;
                    }
                }
                if (pushOut != 0 && (push[0] != 0.0f || push[1] != 0.0f || push[2] != 0.0f)) {
                    MoveCollideResize(push, push, 0.200000003f);
                    MoveCollideAdd(self->tiltPush, self->tiltPush, push);
                }
            }
        }
        pos[1] = pos[1] + delta[1];
        delta[1] = -0.100000001f;
    }

    if (!(delta[1] <= 0.0f)) {
        pos[1] = pos[1] + delta[1];
    } else if ((self->stateFlags & 8) != 0) {
        pos[1] = pos[1] + delta[1];
        if (pos[1] < self->groundY) {
            pos[1] = self->groundY;
            self->stateFlags = self->stateFlags & 0xbfffffffu;
            result = 1;
        }
    } else {
        MoveCollideCopy(sweptStart, pos);
        swept->start.y = swept->start.y + self->radius;
        swept->dir.x = 0.0f;
        swept->dir.y = delta[1];
        swept->dir.z = 0.0f;
        swept->dir.w = 0.0f;
        swept->radius = self->radius;
        swept->start.w = self->radius * self->radius;
        floor = CollisionRaycast(self->collisionMask, swept->shapeBlock, 2);
        self->contactObj = floor;
        if (floor == NULL) {
            pos[1] = pos[1] + delta[1];
        } else {
            const ScePspFVector4 *normal = &g_collisionHitResult.normal;
            bool climbable;
            bool flatEnough;
            bool longAir;
            bool fastFall;
            bool highUp;
            bool land;

            pos[1] = pos[1] - (g_collisionHitInfo.bestDist - 0.00100000005f);
            climbable = g_collisionHitInfo.surface >= 7 && g_collisionHitInfo.surface < 0xb;
            flatEnough = normal->x * normal->x + normal->z * normal->z < normal->y * normal->y;
            longAir = self->airborneFrames >= 0x1a;
            fastFall = self->base.velocity[1] < -20.0f;
            highUp = self->groundY + 50.0f < pos[1];
            if (highUp) {
                resolved = 0;
            }
            if ((highUp && !longAir) || !falling) {
                land = (self->flags & 0x2000) != 0;
            } else if (!climbable && flatEnough) {
                land = true;
            } else if (longAir && fastFall) {
                land = true;
            } else if (pos[1] - self->groundY < 5.0f) {
                land = true;
            } else if (!climbable) {
                land = (self->flags & 0x2000) != 0;
            } else if (!(normal->y <= 0.779999971f)) {
                land = true;
            } else {
                land = (self->flags & 0x2000) != 0;
            }
            if (land) {
                if (longAir && fastFall) {
                    self->flags = self->flags | 0x2000;
                }
                self->stateFlags = self->stateFlags & 0xbfffffffu;
                result = 1;
            }

            if (normal->y <= 0.779999971f && resolved == 0 && result == 0) {
                if (normal->x * normal->x + normal->z * normal->z < 9.99999975e-06f) {
                    alongY = !(normal->y * normal->y <= 9.99999975e-05f);
                } else {
                    alongY = false;
                }
                if (!alongY) {
                    /* slide = normal turned -pi/2 about the horizontal tangent (downhill). */
                    slide[0] = -normal->z;
                    slide[1] = 0.0f;
                    slide[2] = normal->x;
                    MoveCollideNormalise(slide, slide);
                    MoveCollideAxisAngleQuat(slideQuat, slide, -1.57079637f);
                    MoveCollideQuatRotate(slide, slideQuat, hitNormal);
                    MoveCollideNormalise(moveDir, delta);
                    value = (MoveCollideDot(moveDir, slide) - 0.400000006f) * 24.0f;
                    if (value < 0.0f) {
                        value = 0.0f;
                    }
                    MoveCollideScale(slide, slide, value);
                    MoveCollideAdd(self->tiltPush, self->tiltPush, slide);
                    if (limitPush != 0) {
                        MoveCollideNormalise(pushDir, self->tiltPush);
                        MoveCollideNormalise(velDir, self->base.velocity);
                        if (!(MoveCollideDot(pushDir, velDir) < 0.0f)) {
                            MoveCollideCopy(slide, self->base.velocity);
                            slide[1] = 0.0f;
                            MoveCollideSub(self->tiltPush, self->tiltPush, slide);
                            self->base.velocity[0] = self->base.velocity[0] * 0.899999976f;
                            self->base.velocity[2] = self->base.velocity[2] * 0.899999976f;
                        }
                    }
                }
            }
        }
    }

    if (hitWall != 0 && falling != 0) {
        /* vcmp.q EQ over all four lanes */
        if (!(pos[0] == startPos[0] && pos[1] == startPos[1] && pos[2] == startPos[2] &&
              pos[3] == startPos[3]) &&
            (self->isPlayer != 0 || (self->stateFlags & 0x4040010) != 0)) {
            /* Sweep a half-radius sphere along the whole move. */
            MoveCollideCopy(sweptStart, startPos);
            MoveCollideSub(sweptDir, pos, startPos);
            value = MoveCollideLength(sweptDir) + sphere->radius * 0.49000001f;
            MoveCollideResize(sweptDir, sweptDir, value);
            swept->start.y = swept->start.y + sphere->radius;
            value = sphere->radius * 0.5f;
            swept->radius = value;
            swept->start.w = value * value;
            if (CollisionRaycast(self->collisionMask, swept->shapeBlock, 0) != NULL) {
                MoveCollideCopy(pos, startPos);
                self->stateFlags = self->stateFlags & 0xbfffffffu;
                result = 1;
            }
        }
    }
    return result;
}
