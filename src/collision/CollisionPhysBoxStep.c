// bdc 0x089e6fc0 CollisionPhysBoxStep
#include "bdc.h"

/* Simulation step of the physics box (vtable slot `+0x14`): integrates the corner velocities and
   applies `gravity` to unpinned corners (clearing the pins), relaxes the edges
   (`CollisionPhysBoxSolveEdges`), then per corner resolves ground penetration against `floorY`
   or, while `raycastGround` is set, against a downward `CollisionRaycast` from 5 units above the
   corner (a miss clears `raycastGround`; the highest ground found becomes `floorY`). A penetrating
   corner is pinned, pushed up and the push is shared with the other corners; if its velocity points
   back towards the box centre it bounces (velocity reflected off the hit normal, or `y` negated on
   the fixed floor) and the impulse is added to its three `g_collisionPhysBoxNeighbours`. After
   the pinned edge pass (`CollisionPhysBoxSolveEdgesPinned`) it rebuilds the shape around the new
   centroid; with `groundFlag` set it sweeps the box sphere from the old to the new centre and
   slides/bounces it off the world. Finally it derives the velocities
   (`CollisionPhysBoxUpdateVelocities`), rebuilds `mtx` from the axes and the centroid, updates
   `centre`/`prevCentre`/`contactVec`, increments `moveSteps` and returns `|contactVec|^2`.

   VFPU lift: the per-corner offsets start as the bank's C720 (0, 0, 0, 0); S713 (0) is the
   zero-length fallback of every `vcmovt` and the `w` of every `vpfxd`-masked `vscl.t` result. */

static inline float PhysBoxDot3(const ScePspFVector4 *a, const ScePspFVector4 *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

/* vrsq of |v|^2, 0 for a zero-length vector (vcmp EZ + vcmovt S713) */
static inline float PhysBoxInvLen(const ScePspFVector4 *v)
{
    float len2 = PhysBoxDot3(v, v);

    return len2 == 0.0f ? 0.0f : VfRsq(len2);
}

/* vpfxd [-1:1,-1:1,-1:1,M] + vscl.t into C710: w is S713 (0) */
static inline void PhysBoxScaleSat(ScePspFVector4 *out, const ScePspFVector4 *v, float k)
{
    out->x = VfSat1(v->x * k);
    out->y = VfSat1(v->y * k);
    out->z = VfSat1(v->z * k);
    out->w = 0.0f;
}

/* vscl.t into C710: w is S713 (0) */
static inline void PhysBoxScale(ScePspFVector4 *out, const ScePspFVector4 *v, float k)
{
    out->x = v->x * k;
    out->y = v->y * k;
    out->z = v->z * k;
    out->w = 0.0f;
}

/* vadd.t: w kept */
static inline void PhysBoxAddTo(ScePspFVector4 *acc, const ScePspFVector4 *add)
{
    acc->x = acc->x + add->x;
    acc->y = acc->y + add->y;
    acc->z = acc->z + add->z;
}

/* out = normalise(v - 2 * dot(v, n) * n), zero length -> 0, clamped, w = 0 */
static inline void PhysBoxReflect(ScePspFVector4 *out, const ScePspFVector4 *v, const ScePspFVector4 *n)
{
    ScePspFVector4 r;
    float k;

    k = -2.0f * PhysBoxDot3(v, n);
    r.x = v->x + n->x * k;
    r.y = v->y + n->y * k;
    r.z = v->z + n->z * k;
    PhysBoxScaleSat(out, &r, PhysBoxInvLen(&r));
}

float CollisionPhysBoxStep(CollisionPhysBox *self)
{
    ScePspFVector4 contact;    /* sp+0x00 */
    ScePspFVector4 centroid;   /* sp+0x10 */
    ScePspFVector4 offsets[8]; /* sp+0x20 */
    ScePspFVector4 impulse;    /* sp+0xa0 outward dir, then impulse */
    ScePspFVector4 velDir;     /* sp+0xb0 */
    ScePspFVector4 rayStart;   /* sp+0xc0 */
    ScePspFVector4 tmp;
    ScePspFVector4 delta;      /* sp+0xf0 */
    ScePspFVector4 movePos;    /* sp+0x100 */
    ScePspFVector4 deltaDir;   /* sp+0x130 */
    float maxGroundY;
    float depth;
    float half;
    float quarter;
    float scale;
    s32 i;
    s32 j;

    /* integrate, gravity on unpinned corners, clear pins */
    for (i = 0; i < 8; i++) {
        PhysBoxAddTo(&self->pos[i], &self->vel[i]);
        if (self->pins[i] != 0) {
            self->pins[i] = 0;
        } else {
            self->pos[i].y = self->pos[i].y - self->gravity;
        }
    }
    CollisionPhysBoxSolveEdges(self, 4);
    for (i = 0; i < 8; i++) {
        offsets[i].x = 0.0f;
        offsets[i].y = 0.0f;
        offsets[i].z = 0.0f;
        offsets[i].w = 0.0f;
    }

    maxGroundY = g_collisionPhysBoxNegInf;
    for (i = 0; i < 8; i++) {
        const u8 *nb = &g_collisionPhysBoxNeighbours[i * 3];

        if (self->raycastGround != 0) {
            ScePspFVector4 *d = &g_collisionRayDesc.dir;

            rayStart = self->pos[i];
            rayStart.y = rayStart.y + 5.0f;
            g_collisionRayDesc.origin = rayStart;
            g_collisionRayDesc.dir = g_vecDown;
            g_collisionRayDesc.invDir.x = d->x == 0.0f ? 0.0f : VfRcp(d->x);
            g_collisionRayDesc.invDir.y = d->y == 0.0f ? 0.0f : VfRcp(d->y);
            g_collisionRayDesc.invDir.z = d->z == 0.0f ? 0.0f : VfRcp(d->z);
            g_collisionRayDesc.invDir.w = 0.0f;
            if (CollisionRaycast(0x3fbf2700, &g_collisionRayBlock, 0) == NULL) {
                float floorY = self->floorY;

                depth = self->pos[i].y - floorY;
                if (maxGroundY < floorY) {
                    maxGroundY = self->floorY;
                }
                self->raycastGround = 0;
            } else {
                float hitY = g_collisionHitResult.point.y;

                depth = self->pos[i].y - hitY;
                if (maxGroundY < hitY) {
                    maxGroundY = g_collisionHitResult.point.y;
                }
            }
        } else {
            depth = self->pos[i].y - self->floorY;
        }
        if (!(depth < 0.0f)) {
            continue;
        }

        /* push the corner out, share the push with the others */
        self->pins[i] = 1;
        self->pos[i].y = self->pos[i].y - depth;
        half = depth * 0.5f;
        quarter = half * 0.5f;
        for (j = 0; j < 8; j++) {
            /* UB (original binary): reads 8 bytes from row i, past the table for i >= 6 */
            if ((s32)nb[j] == j) {
                self->pos[j].y = self->pos[j].y - half;
            } else {
                self->pos[j].y = self->pos[j].y - quarter;
            }
        }
        /* all three velocity lanes +-0 (bit test with 0x7fffffff) */
        if (self->vel[i].x == 0.0f && self->vel[i].y == 0.0f && self->vel[i].z == 0.0f) {
            continue;
        }

        /* bounce: -1.2 * dot(dir from centre, velocity dir) */
        offsets[i] = self->vel[i];
        tmp.x = self->pos[i].x - self->centre.x;
        tmp.y = self->pos[i].y - self->centre.y;
        tmp.z = self->pos[i].z - self->centre.z;
        PhysBoxScaleSat(&impulse, &tmp, PhysBoxInvLen(&tmp));
        PhysBoxScaleSat(&velDir, &self->vel[i], PhysBoxInvLen(&self->vel[i]));
        scale = PhysBoxDot3(&impulse, &velDir) * -1.2f;
        if (scale <= 0.0f) {
            self->stepCounter = self->stepCounter + 1;
            continue;
        }
        if (self->raycastGround == 0) {
            offsets[i].y = -offsets[i].y;
        } else {
            ScePspFVector4 *n = &g_collisionHitResult.normal;
            float speed;

            n->x = -n->x;
            n->y = -n->y;
            n->z = -n->z;
            speed = __builtin_sqrtf(PhysBoxDot3(&self->vel[i], &self->vel[i]));
            PhysBoxReflect(&offsets[i], &self->vel[i], n);
            scale = scale * speed;
            scale = scale * 0.6f;
        }
        PhysBoxScale(&impulse, &offsets[i], scale);
        if (offsets[i].w == 0.0f) {
            for (j = 0; j < 3; j++) {
                PhysBoxAddTo(&offsets[nb[j]], &impulse);
            }
            offsets[i].w = 1.0f;
        }
        self->stepCounter = self->stepCounter + 1;
    }
    if (self->raycastGround != 0) {
        self->floorY = maxGroundY;
    }

    CollisionPhysBoxSolveEdgesPinned(self, 4);
    CollisionPhysBoxCentroid(self, &centroid);
    CollisionPhysBoxRebuildShape(self, &centroid);

    if (self->groundFlag != 0 && self->moveSteps != 0) {
        float dist;

        delta = centroid;
        delta.x = centroid.x - self->centre.x;
        delta.y = centroid.y - self->centre.y;
        delta.z = centroid.z - self->centre.z;
        movePos = self->centre;
        dist = __builtin_sqrtf(PhysBoxDot3(&delta, &delta));
        if (!(dist < 0.001f)) {
            g_collisionSweptSphereDesc.radius = self->radius;
            g_collisionSweptSphereDesc.start = movePos;
            g_collisionSweptSphereDesc.dir = delta;
            if (CollisionRaycast(0x3fbf2700, g_collisionSweptSphereDesc.shapeBlock, 0) != NULL) {
                float cosv;
                float k;
                const VtblEntry *e;

                /* slide to the hit, then reflect the remaining motion off the normal */
                PhysBoxScaleSat(&deltaDir, &delta, PhysBoxInvLen(&delta));
                PhysBoxScale(&tmp, &delta, g_collisionHitInfo.bestT - 0.01f);
                PhysBoxAddTo(&movePos, &tmp);
                PhysBoxReflect(&delta, &delta, &g_collisionHitResult.normal);
                cosv = PhysBoxDot3(&delta, &deltaDir);
                cosv = (cosv - 1.0f) * 0.7f;
                if (cosv < -1.0f) {
                    cosv = -1.0f;
                }
                k = cosv * 0.4f + 1.0f;
                k = dist * k;
                k = k * 0.97f;
                PhysBoxScale(&delta, &delta, k);
                PhysBoxScale(&tmp, &delta, 1.0f - g_collisionHitInfo.bestT);
                PhysBoxScale(&tmp, &tmp, cosv + 1.0f);
                PhysBoxAddTo(&movePos, &tmp);

                /* push the box sphere out of the world at the new position */
                g_btlBakuganSphereQuery.radius = self->radius;
                g_btlBakuganSphereQuery.center = movePos;
                e = &g_btlBakuganSphereQuery.vtbl[9]; /* CollisionSphereRecalc */
                ((void (*)(void *))e->fn)((u8 *)&g_btlBakuganSphereQuery + e->delta);
                if (CollisionRaycast(0x3fbf2700, &g_collisionSphereBlock, 0) != NULL) {
                    PhysBoxScale(&tmp, &g_collisionHitResult.normal, self->radius + 0.01f);
                    movePos = g_collisionHitResult.point;
                    PhysBoxAddTo(&movePos, &tmp);
                }
                delta = movePos;
                delta.x = movePos.x - centroid.x;
                delta.y = movePos.y - centroid.y;
                delta.z = movePos.z - centroid.z;
                for (j = 0; j < 8; j++) {
                    PhysBoxAddTo(&offsets[j], &delta);
                    PhysBoxAddTo(&self->pos[j], &delta);
                }
                centroid = movePos;
            }
        }
    }

    CollisionPhysBoxUpdateVelocities(self, offsets);

    /* mtx rotation from axes[2] (z) and axes[1] (up), as MathMat4LookDir */
    {
        ScePspFVector4 ax;
        ScePspFVector4 ay;
        ScePspFVector4 az;
        const ScePspFVector4 *up = &self->axes[1];

        PhysBoxScaleSat(&az, &self->axes[2], PhysBoxInvLen(&self->axes[2]));
        ax.x = up->y * az.z - up->z * az.y;
        ax.y = up->z * az.x - up->x * az.z;
        ax.z = up->x * az.y - up->y * az.x;
        PhysBoxScaleSat(&ax, &ax, PhysBoxInvLen(&ax));
        ay.x = az.y * ax.z - az.z * ax.y;
        ay.y = az.z * ax.x - az.x * ax.z;
        ay.z = az.x * ax.y - az.y * ax.x;
        ay.w = 0.0f;
        az.w = 0.0f; /* vidt.q R003 */
        self->mtx->x = ax;
        self->mtx->y = ay;
        self->mtx->z = az;
        self->mtx->w.x = 0.0f; /* vidt.q C030 */
        self->mtx->w.y = 0.0f;
        self->mtx->w.z = 0.0f;
        self->mtx->w.w = 1.0f;
    }

    contact = centroid;
    contact.x = centroid.x - self->centre.x;
    contact.y = centroid.y - self->centre.y;
    contact.z = centroid.z - self->centre.z;
    self->contactVec = contact;
    self->prevCentre = self->centre;
    self->centre = centroid;
    centroid.w = 1.0f;
    self->mtx->w = centroid;
    self->moveSteps = self->moveSteps + 1;

    return PhysBoxDot3(&self->contactVec, &self->contactVec);
}
