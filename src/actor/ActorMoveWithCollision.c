// bdc 0x088dd2dc ActorMoveWithCollision
#include "bdc.h"

/* vcmp.s EZ + vrsq.s + vcmovt.s S713: 1/sqrt(d), or 0 (S713) for a zero length. */
static float ActorMoveRsqOrZero(float d)
{
    return d == 0.0f ? 0.0f : VfRsq(d);
}

/* vqmul.q d, a, b (w last). */
static void ActorMoveQuatMul(ScePspFVector4 *d, const ScePspFVector4 *a, const ScePspFVector4 *b)
{
    d->x = a->x * b->w + a->y * b->z - a->z * b->y + a->w * b->x;
    d->y = -a->x * b->z + a->y * b->w + a->z * b->x + a->w * b->y;
    d->z = a->x * b->y - a->y * b->x + a->z * b->w + a->w * b->z;
    d->w = -a->x * b->x - a->y * b->y - a->z * b->z + a->w * b->w;
}

/* q * (v.xyz, 0) * conj(q) (S730 = 0 as the w lane of v). */
static void ActorMoveQuatRotate(ScePspFVector4 *d, const ScePspFVector4 *q, const float *v)
{
    ScePspFVector4 conj;
    ScePspFVector4 p;
    ScePspFVector4 t;

    conj.x = -q->x;
    conj.y = -q->y;
    conj.z = -q->z;
    conj.w = q->w;
    p.x = v[0];
    p.y = v[1];
    p.z = v[2];
    p.w = 0.0f;
    ActorMoveQuatMul(&t, q, &p);
    ActorMoveQuatMul(d, &t, &conj);
}

/* (axis.xyz * sin(a), cos(a)) with a = angle / 2 (vcst 1/PI times the angle, in quarter turns). */
static void ActorMoveAxisAngleQuat(ScePspFVector4 *d, const ScePspFVector4 *axis, float angle)
{
    float q;
    float s;

    q = 0.318309873f * angle;
    d->w = VfCosQuarter(q);
    s = VfSinQuarter(q);
    d->x = axis->x * s;
    d->y = axis->y * s;
    d->z = axis->z * s;
}

/* xyz normalised and clamped to [-1, 1] (vpfxd [-1:1,-1:1,-1:1,M]), w = S713 (0). */
static void ActorMoveNormalizeSat(ScePspFVector4 *d, const float *v)
{
    float k;

    k = ActorMoveRsqOrZero(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    d->x = VfSat1(v[0] * k);
    d->y = VfSat1(v[1] * k);
    d->z = VfSat1(v[2] * k);
    d->w = 0.0f;
}

/* Moves an actor by the displacement `delta` against the collision world and returns 1 when it
   lands on the ground this step (0 otherwise). Returns 1 at once when `g_actorMoveDepth` has
   reached 5 (bumped by every call, reset only by `ActorMoveUnguarded`). The step gets
   `extraMove` added (then zeroed) and, when the actor was grounded on entry, rotated by the
   ground-tilt quaternion (identity on flat ground). Steps longer than radius / 1.05f first recurse
   on up to three successive halves; then the actor is moved, pushed apart from the others
   (`ActorSeparateFromActors`), swept horizontally with the sphere query
   `g_btlBakuganSphereQuery` (`CollisionRaycast`, `ActorSlideOffHit`) and vertically with the
   swept sphere `g_collisionSweptSphereDesc` (ceiling mode 4 with the "Bip01_Head" bone, ground
   mode 2). VFPU bank constants: C720 = 0, C730 = (0, 0, 0, 1), S713 = 0. */
int ActorMoveWithCollision(Actor *self, float *delta)
{
    float move[4];
    float startPos[4];
    ScePspFVector4 tilt;
    ScePspFVector4 axis;
    float half1[4];
    float half2[4];
    float half3[4];
    float off[4];
    ScePspFVector4 head;
    ScePspFVector4 tangent;
    ScePspFVector4 quat;
    ScePspFVector4 dirN;
    ScePspFVector4 pushN;
    ScePspFVector4 velN;
    CollisionSphereQuery *sphere;
    CollisionSweptSphereDesc *swept;
    const VtblEntry *e;
    void *hit;
    u8 landed;
    s32 wasAirborne;
    s32 flat;
    s32 blocked;
    s32 steep;
    u8 falling;
    float len;
    float len2;
    float r;
    float k;
    float dist;
    float dot;
    float scale;

    if (!(g_actorMoveDepth < 5)) {
        return 1;
    }
    g_actorMoveDepth = g_actorMoveDepth + 1;
    landed = 0;
    wasAirborne = (self->flags & 0x40000000) != 0;
    self->flags = self->flags | 0x40000000;
    move[0] = delta[0];
    move[1] = delta[1];
    move[2] = delta[2];
    move[3] = delta[3];
    startPos[0] = self->base.pos[0];
    startPos[1] = self->base.pos[1];
    startPos[2] = self->base.pos[2];
    startPos[3] = self->base.pos[3];
    move[0] = move[0] + self->extraMove[0];
    move[1] = move[1] + self->extraMove[1];
    move[2] = move[2] + self->extraMove[2];
    delta[1] = delta[1] + self->extraMove[1];
    move[1] = 0.0f;

    flat = 0;
    if (self->groundNormal[0] * self->groundNormal[0] +
            self->groundNormal[2] * self->groundNormal[2] <
        1e-05f) {
        if (!(self->groundNormal[1] * self->groundNormal[1] <= 0.0001f)) {
            flat = 1;
        }
    }
    if (flat != 0) {
        /* C730 */
        tilt.x = 0.0f;
        tilt.y = 0.0f;
        tilt.z = 0.0f;
        tilt.w = 1.0f;
    } else {
        axis.x = self->groundNormal[2];
        axis.y = 0.0f;
        axis.z = -self->groundNormal[0];
        axis.w = 0.0f; /* never written in the original; only reaches a dead lane */
        ActorMoveNormalizeSat(&axis, &axis.x);
        ActorMoveAxisAngleQuat(&tilt, &axis, acosf(self->groundNormal[1]));
    }
    if (wasAirborne == 0) {
        ActorMoveQuatRotate(&quat, &tilt, move);
        move[0] = quat.x;
        move[1] = quat.y;
        move[2] = quat.z;
        move[3] = quat.w;
    }
    /* C720 */
    self->extraMove[0] = 0.0f;
    self->extraMove[1] = 0.0f;
    self->extraMove[2] = 0.0f;
    self->extraMove[3] = 0.0f;
    len = __builtin_sqrtf(move[0] * move[0] + move[1] * move[1] + move[2] * move[2]);
    if (self->radius < len * 1.05f) {
        move[0] = move[0] * 0.5f;
        move[1] = move[1] * 0.5f;
        move[2] = move[2] * 0.5f;
        move[3] = 0.0f;
        half1[0] = move[0];
        half1[1] = move[1];
        half1[2] = move[2];
        half1[3] = move[3];
        ActorMoveWithCollision(self, half1);
        len2 = len * 0.5f;
        if (self->radius < len2 * 1.05f) {
            move[0] = move[0] * 0.5f;
            move[1] = move[1] * 0.5f;
            move[2] = move[2] * 0.5f;
            move[3] = 0.0f;
            half2[0] = move[0];
            half2[1] = move[1];
            half2[2] = move[2];
            half2[3] = move[3];
            ActorMoveWithCollision(self, half2);
            if (self->radius < len2 * 0.5f * 1.05f) {
                move[0] = move[0] * 0.5f;
                move[1] = move[1] * 0.5f;
                move[2] = move[2] * 0.5f;
                move[3] = 0.0f;
                half3[0] = move[0];
                half3[1] = move[1];
                half3[2] = move[2];
                half3[3] = move[3];
                ActorMoveWithCollision(self, half3);
            }
        }
    }
    swept = &g_collisionSweptSphereDesc;
    self->base.pos[0] = self->base.pos[0] + move[0];
    self->base.pos[1] = self->base.pos[1] + move[1];
    self->base.pos[2] = self->base.pos[2] + move[2];
    swept->dir.x = move[0];
    swept->dir.y = move[1];
    swept->dir.z = move[2];
    swept->dir.w = move[3];
    ActorSeparateFromActors(self, move);

    if (self->isPlayer != 0) {
        if (g_scriptGlobalVars[1] == 8) {
            r = self->radius * 0.8f;
        } else {
            r = self->radius;
        }
    } else {
        e = &((const VtblEntry *)self->base.base.vtable)[19];
        if (((s32(*)(void *))e->fn)((u8 *)self + e->delta) != 0) {
            r = self->radius * 0.8f;
        } else {
            r = self->radius;
        }
    }
    sphere = &g_btlBakuganSphereQuery;
    sphere->radius = r;
    blocked = 1;
    /* C720 */
    self->sweepBack[0] = 0.0f;
    self->sweepBack[1] = 0.0f;
    self->sweepBack[2] = 0.0f;
    self->sweepBack[3] = 0.0f;
    if (move[0] != 0.0f || move[2] != 0.0f) {
        self->sweepBack[0] = self->base.pos[0];
        self->sweepBack[1] = self->base.pos[1];
        self->sweepBack[2] = self->base.pos[2];
        self->sweepBack[3] = self->base.pos[3];
        blocked = 0;
        sphere->center.x = self->base.pos[0];
        sphere->center.y = self->base.pos[1];
        sphere->center.z = self->base.pos[2];
        sphere->center.w = self->base.pos[3];
        sphere->center.y = sphere->center.y + sphere->radius;
        e = &sphere->vtbl[9];
        ((void (*)(void *))e->fn)((u8 *)sphere + e->delta);
        e = &((const VtblEntry *)self->base.base.vtable)[19];
        if (((s32(*)(void *))e->fn)((u8 *)self + e->delta) != 0) {
            sphere->radius = self->radius;
        }
        hit = CollisionRaycast(self->collisionMask, &g_collisionSphereBlock, 1);
        if (hit != NULL) {
            blocked = 1;
            if (((CollisionCollider *)hit)->layer == 8) {
                self->flags = self->flags | 0x200;
            }
            ActorSlideOffHit(sphere, self->base.pos, move, 0);
            sphere->center.x = self->base.pos[0];
            sphere->center.y = self->base.pos[1];
            sphere->center.z = self->base.pos[2];
            sphere->center.w = self->base.pos[3];
            sphere->center.y = sphere->center.y + sphere->radius;
            e = &sphere->vtbl[9];
            ((void (*)(void *))e->fn)((u8 *)sphere + e->delta);
            if (CollisionRaycast(self->collisionMask, &g_collisionSphereBlock, 1) != NULL) {
                if (fabsf(g_collisionHitResult.normal.y) < 0.1f) {
                    self->flags = self->flags | 0x20000;
                }
                ActorSlideOffHit(sphere, self->base.pos, move, 0);
                sphere->center.x = self->base.pos[0];
                sphere->center.y = self->base.pos[1];
                sphere->center.z = self->base.pos[2];
                sphere->center.w = self->base.pos[3];
                sphere->center.y = sphere->center.y + sphere->radius;
                e = &sphere->vtbl[9];
                ((void (*)(void *))e->fn)((u8 *)sphere + e->delta);
                if (CollisionRaycast(self->collisionMask, &g_collisionSphereBlock, 1) != NULL) {
                    self->base.pos[0] = startPos[0];
                    self->base.pos[1] = startPos[1];
                    self->base.pos[2] = startPos[2];
                    self->base.pos[3] = startPos[3];
                }
            }
        }
        self->sweepBack[0] = self->sweepBack[0] - self->base.pos[0];
        self->sweepBack[1] = self->sweepBack[1] - self->base.pos[1];
        self->sweepBack[2] = self->sweepBack[2] - self->base.pos[2];
    }
    if (blocked == 0) {
        if (self->radius < 6.0f) {
            r = self->radius + 0.1f;
            if (!(r <= 6.0f)) {
                r = 6.0f;
            }
            self->radius = r;
        }
    }

    falling = delta[1] <= 0.0f;
    sphere->center.x = self->base.pos[0];
    sphere->center.y = self->base.pos[1];
    sphere->center.z = self->base.pos[2];
    sphere->center.w = self->base.pos[3];
    sphere->center.y = sphere->center.y + sphere->radius;
    e = &sphere->vtbl[9];
    ((void (*)(void *))e->fn)((u8 *)sphere + e->delta);

    if (falling == 0) {
        /* ceiling probe */
        swept->radius = self->radius - 13.0f;
        swept->start.x = self->base.pos[0];
        swept->start.y = self->base.pos[1];
        swept->start.z = self->base.pos[2];
        swept->start.w = self->base.pos[3];
        swept->start.y = self->radius - 13.0f + delta[1] + swept->start.y;
        r = self->stepHeight + delta[1];
        swept->dir.x = 0.0f;
        swept->dir.y = r;
        swept->dir.z = 0.0f;
        swept->dir.w = 0.0f;
        hit = CollisionRaycast(self->collisionMask, swept->shapeBlock, 4);
        if (hit != NULL && self->base.pos[1] + 1.0f < g_collisionHitResult.point.y) {
            /* off = (start + dir) - hit point, horizontal */
            off[0] = (swept->start.x + swept->dir.x) - g_collisionHitResult.point.x;
            off[1] = 0.0f;
            off[2] = (swept->start.z + swept->dir.z) - g_collisionHitResult.point.z;
            dist = __builtin_sqrtf(off[0] * off[0] + off[1] * off[1] + off[2] * off[2]);
            if (dist < (self->radius - 13.0f) * 0.5f) {
                delta[1] = ((swept->start.y + g_collisionHitInfo.bestDist) -
                            ((self->radius - 13.0f) + self->stepHeight)) -
                           self->base.pos[1];
                blocked = 1;
                GfxModelGetNodeWorldPos(&self->base, &head, "Bip01_Head");
                /* off = pos - hit point, horizontal */
                off[0] = self->base.pos[0] - g_collisionHitResult.point.x;
                off[1] = 0.0f;
                off[2] = self->base.pos[2] - g_collisionHitResult.point.z;
                dist = __builtin_sqrtf(off[0] * off[0] + off[1] * off[1] + off[2] * off[2]);
                /* Both "head close" branches below also compute a camera-facing head offset and
                   its distance to the hit point; nothing reads them (blocked becomes 0), so they
                   are left out. */
                if (dist < (self->radius - 13.0f) * 0.4f) {
                    blocked = 0;
                    self->base.velocity[1] = 0.0f;
                } else {
                    /* off = normalize(off) * 0.5f (no clamp, w = S713) */
                    k = ActorMoveRsqOrZero(off[0] * off[0] + off[1] * off[1] + off[2] * off[2]);
                    k = k * 0.5f;
                    off[0] = off[0] * k;
                    off[1] = off[1] * k;
                    off[2] = off[2] * k;
                    off[3] = 0.0f;
                    g_collisionSegmentDesc.start[0] = g_collisionHitResult.point.x + off[0];
                    g_collisionSegmentDesc.start[1] = g_collisionHitResult.point.y + off[1];
                    g_collisionSegmentDesc.start[2] = g_collisionHitResult.point.z + off[2];
                    g_collisionSegmentDesc.start[3] = g_collisionHitResult.point.w;
                    g_collisionSegmentDesc.start[1] = g_collisionSegmentDesc.start[1] - 3.0f;
                    g_collisionSegmentDesc.dir[0] = g_vecUp.x * 8.0f;
                    g_collisionSegmentDesc.dir[1] = g_vecUp.y * 8.0f;
                    g_collisionSegmentDesc.dir[2] = g_vecUp.z * 8.0f;
                    g_collisionSegmentDesc.dir[3] = 0.0f;
                    if (CollisionRaycast(self->collisionMask, &g_collisionSegmentBlock, 0) !=
                        NULL) {
                        blocked = 0;
                        self->base.velocity[1] = 0.0f;
                    }
                }
                if (blocked != 0 && (off[0] != 0.0f || off[1] != 0.0f || off[2] != 0.0f)) {
                    /* off = normalize(off) * 0.2f; extraMove.xyz += off */
                    k = ActorMoveRsqOrZero(off[0] * off[0] + off[1] * off[1] + off[2] * off[2]);
                    k = k * 0.2f;
                    off[0] = off[0] * k;
                    off[1] = off[1] * k;
                    off[2] = off[2] * k;
                    self->extraMove[0] = self->extraMove[0] + off[0];
                    self->extraMove[1] = self->extraMove[1] + off[1];
                    self->extraMove[2] = self->extraMove[2] + off[2];
                }
            }
        }
    }

    if (delta[1] <= 0.0f) {
        if ((self->flags & 8) != 0) {
            self->base.pos[1] = self->base.pos[1] + delta[1];
            if (self->base.pos[1] < self->groundPoint[1]) {
                self->base.pos[1] = self->groundPoint[1];
                self->flags = self->flags & 0xbfffffff;
                landed = 1;
            }
        } else {
            /* ground probe */
            swept->start.x = self->base.pos[0];
            swept->start.y = self->base.pos[1];
            swept->start.z = self->base.pos[2];
            swept->start.w = self->base.pos[3];
            swept->start.y = swept->start.y + self->radius;
            r = delta[1];
            swept->dir.x = 0.0f;
            swept->dir.y = r;
            swept->dir.z = 0.0f;
            swept->dir.w = 0.0f;
            r = self->radius;
            swept->radius = r;
            swept->start.w = r * r;
            hit = CollisionRaycast(self->collisionMask, swept->shapeBlock, 2);
            if (hit == NULL) {
                self->base.pos[1] = self->base.pos[1] + delta[1];
            } else {
                if (delta[1] <= 0.0f) {
                    self->base.pos[1] =
                        self->base.pos[1] - (g_collisionHitInfo.bestDist - 0.0001f);
                }
                if (g_collisionHitInfo.fromClosestPoint != 0) {
                    flat = 0;
                    if (g_collisionHitResult.normal.x * g_collisionHitResult.normal.x +
                            g_collisionHitResult.normal.z * g_collisionHitResult.normal.z <
                        1e-05f) {
                        if (!(g_collisionHitResult.normal.y * g_collisionHitResult.normal.y <=
                              0.0001f)) {
                            flat = 1;
                        }
                    }
                    if (flat == 0) {
                        /* axis (-n.z, 0, n.x) normalised; quat = rotation of -pi/2 about it;
                           tangent = quat * (normal.xyz, 0) * conj(quat) */
                        axis.x = -g_collisionHitResult.normal.z;
                        axis.y = 0.0f;
                        axis.z = g_collisionHitResult.normal.x;
                        axis.w = g_collisionHitResult.normal.w;
                        ActorMoveNormalizeSat(&axis, &axis.x);
                        ActorMoveAxisAngleQuat(&quat, &axis, -1.57079637f);
                        ActorMoveQuatRotate(&tangent, &quat, &g_collisionHitResult.normal.x);
                        ActorMoveNormalizeSat(&dirN, delta);
                        dot = dirN.x * tangent.x + dirN.y * tangent.y + dirN.z * tangent.z;
                        scale = dot - 0.2f;
                        if (scale < 0.0f) {
                            scale = 0.0f;
                        }
                        tangent.x = tangent.x * scale;
                        tangent.y = tangent.y * scale;
                        tangent.z = tangent.z * scale;
                        self->extraMove[0] = self->extraMove[0] + tangent.x;
                        self->extraMove[1] = self->extraMove[1] + tangent.y;
                        self->extraMove[2] = self->extraMove[2] + tangent.z;
                        if (wasAirborne != 0) {
                            ActorMoveNormalizeSat(&pushN, self->extraMove);
                            ActorMoveNormalizeSat(&velN, self->base.velocity);
                            dot = pushN.x * velN.x + pushN.y * velN.y + pushN.z * velN.z;
                            if (!(dot < 0.0f)) {
                                /* extraMove.xyz -= (velocity.x, 0, velocity.z) */
                                self->extraMove[0] = self->extraMove[0] - self->base.velocity[0];
                                self->extraMove[1] = self->extraMove[1] - 0.0f;
                                self->extraMove[2] = self->extraMove[2] - self->base.velocity[2];
                                self->base.velocity[0] = self->base.velocity[0] * 0.9f;
                                self->base.velocity[2] = self->base.velocity[2] * 0.9f;
                            }
                        }
                    }
                }
                steep = 0;
                if (g_collisionHitInfo.surface >= 7 && g_collisionHitInfo.surface < 11) {
                    steep = 1;
                }
                if (falling != 0) {
                    if (steep == 0 ? !(g_collisionHitResult.normal.y <= 0.3f)
                                   : !(g_collisionHitResult.normal.y <= 0.78f)) {
                        self->flags = self->flags & 0xbfffffff;
                        landed = 1;
                    }
                }
            }
        }
    } else {
        self->base.pos[1] = self->base.pos[1] + delta[1];
    }
    if (wasAirborne != 0 && (self->flags & 0x40000000) == 0) {
        self->flags = self->flags | 0x80000000;
    }
    return landed;
}
