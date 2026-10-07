// bdc 0x089e7944 CollisionPhysBoxInit
#include "bdc.h"

/* Sets up a physics box from an axis-aligned box (`aabb` min/max) and a transform `mtx`: allocates
   the 0x180-byte particle buffer (pos / vel / prevPos, 8 vec4 each) from low heap memory on first
   use, stores half of `CollisionAabbExtent` as `halfExtents` (w = 0) and clamps each axis up to
   0.2, places the eight corners from `g_collisionUnitCorners` scaled by the half extents (offset
   by the box centre `(min + max) / 2` when `centred`), transforms them with
   `MathMtx4TransformPoint` into `pos` (w = 0) and `prevPos`, zeroes `vel`, stores the transform
   and a radius (half the smallest unclamped half extent, starting from `g_collisionPhysBoxInf`),
   runs `CollisionPhysBoxInitConstraints`, then zeroes `centre`, `prevCentre` and `moveSteps`. */
void CollisionPhysBoxInit(CollisionPhysBox *self, void *mtx, const ScePspFVector4 *aabb, bool centred)
{
    ScePspFVector4 point;
    ScePspFVector4 centre;
    ScePspFVector4 out;
    const float *bounds;
    ScePspFVector4 *ext;
    ScePspFVector4 *buf;
    float minExt;
    bool fromLow;
    s32 i;

    minExt = g_collisionPhysBoxInf;
    if (self->pos == NULL) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        buf = MemAlloc(3 * 8 * sizeof(ScePspFVector4), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        self->pos = buf;
        self->vel = buf + 8;
        self->prevPos = buf + 16;
    }
    ext = CollisionAabbExtent(&aabb->x, &self->halfExtents);
    self->halfExtents.x = ext->x * 0.5f;
    self->halfExtents.y = ext->y * 0.5f;
    self->halfExtents.z = ext->z * 0.5f;
    self->halfExtents.w = 0.0f;
    if (!(minExt <= self->halfExtents.x)) {
        minExt = self->halfExtents.x;
    }
    if (!(minExt <= self->halfExtents.y)) {
        minExt = self->halfExtents.y;
    }
    if (!(minExt <= self->halfExtents.z)) {
        minExt = self->halfExtents.z;
    }
    if (self->halfExtents.x < 0.2f) {
        self->halfExtents.x = 0.2f;
    }
    if (self->halfExtents.y < 0.2f) {
        self->halfExtents.y = 0.2f;
    }
    if (self->halfExtents.z < 0.2f) {
        self->halfExtents.z = 0.2f;
    }
    if (centred) {
        /* aabb is min (floats 0..3) followed by max (floats 4..7), as in CollisionAabbExtent */
        bounds = &aabb->x;
        centre.x = (bounds[0] + bounds[4]) * 0.5f;
        centre.y = (bounds[1] + bounds[5]) * 0.5f;
        centre.z = (bounds[2] + bounds[6]) * 0.5f;
        centre.w = 0.0f;
    }
    for (i = 0; i < 8; i++) {
        point.x = g_collisionUnitCorners[i].x * self->halfExtents.x;
        point.y = g_collisionUnitCorners[i].y * self->halfExtents.y;
        point.z = g_collisionUnitCorners[i].z * self->halfExtents.z;
        point.w = 0.0f;
        if (centred) {
            point.x = point.x + centre.x;
            point.y = point.y + centre.y;
            point.z = point.z + centre.z;
        }
        MathMtx4TransformPoint(mtx, &out, &point);
        self->pos[i] = out;
        self->pos[i].w = 0.0f;
        self->prevPos[i] = self->pos[i];
        self->vel[i].x = 0.0f;
        self->vel[i].y = 0.0f;
        self->vel[i].z = 0.0f;
        self->vel[i].w = 0.0f;
    }
    self->mtx = mtx;
    self->radius = minExt * 0.5f;
    CollisionPhysBoxInitConstraints(self);
    self->centre.x = 0.0f;
    self->centre.y = 0.0f;
    self->centre.z = 0.0f;
    self->centre.w = 0.0f;
    self->prevCentre.x = 0.0f;
    self->prevCentre.y = 0.0f;
    self->prevCentre.z = 0.0f;
    self->prevCentre.w = 0.0f;
    self->moveSteps = 0;
}
