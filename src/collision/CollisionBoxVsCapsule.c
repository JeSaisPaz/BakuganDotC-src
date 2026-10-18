// bdc 0x089ea6d4 CollisionBoxVsCapsule
#include "bdc.h"

/* Box shape method for capsule queries (type-6 vtable slot 3): caches the inverse box transform
   (`invTransform`, flag `invValid`; transposed rotation with the negated rotated translation),
   transforms the query into box space through its vtable (slot 6) into the static capsule shape
   `g_collisionBoxCapsuleQueryShape`, runs `CollisionAabbCapsuleContact` against the box extents and,
   on a hit, transforms the point in `out` back to world space (`(out.xyz, 1) * transform`).
   Returns whether it hit. */

bool CollisionBoxVsCapsule(CollisionBox *self, void *capsule, ScePspFVector4 *out)
{
    const VtblEntry *vtbl;
    const ScePspFMatrix4 *m;
    ScePspFMatrix4 *inv;
    float tx, ty, tz;
    float px, py, pz;
    SegmentShape *axisSeg;

    if (g_collisionBoxCapsuleQueryInit == 0) {
        g_collisionBoxCapsuleQueryInit = 1;
        axisSeg = &g_collisionBoxCapsuleQueryShape.segment;
        g_collisionBoxCapsuleQueryShape.vtbl = g_collisionCapsuleVtbl;
        axisSeg->info = (void *)g_collisionSegmentVtbl;
        axisSeg->type = 2;
        g_collisionBoxCapsuleQueryShape.type = 4;
    }
    if (self->invValid == 0) {
        m = &self->transform;
        inv = &self->invTransform;
        /* rotated translation: dot3(axis_i, translation) */
        tx = VfDot3(m->x.x, m->x.y, m->x.z, m->w.x, m->w.y, m->w.z);
        ty = VfDot3(m->y.x, m->y.y, m->y.z, m->w.x, m->w.y, m->w.z);
        tz = VfDot3(m->z.x, m->z.y, m->z.z, m->w.x, m->w.y, m->w.z);
        /* transposed rotation, w lanes 0 */
        inv->x.x = m->x.x;
        inv->x.y = m->y.x;
        inv->x.z = m->z.x;
        inv->x.w = 0.0f;
        inv->y.x = m->x.y;
        inv->y.y = m->y.y;
        inv->y.z = m->z.y;
        inv->y.w = 0.0f;
        inv->z.x = m->x.z;
        inv->z.y = m->y.z;
        inv->z.z = m->z.z;
        inv->z.w = 0.0f;
        inv->w.x = -tx;
        inv->w.y = -ty;
        inv->w.z = -tz;
        inv->w.w = m->w.w;
        self->invValid = 1;
    }
    vtbl = ((const CollisionBox *)capsule)->vtbl + 6;
    ((void (*)(void *, const ScePspFMatrix4 *, CollisionCapsule *))vtbl->fn)(
        (char *)capsule + vtbl->delta, &self->invTransform, &g_collisionBoxCapsuleQueryShape);
    if (CollisionAabbCapsuleContact(&(self->aabbMin).x, &g_collisionBoxCapsuleQueryShape, out)) {
        m = &self->transform;
        px = out->x;
        py = out->y;
        pz = out->z;
        out->x = VfDot4(m->x.x, m->y.x, m->z.x, m->w.x, px, py, pz, 1.0f);
        out->y = VfDot4(m->x.y, m->y.y, m->z.y, m->w.y, px, py, pz, 1.0f);
        out->z = VfDot4(m->x.z, m->y.z, m->z.z, m->w.z, px, py, pz, 1.0f);
        out->w = VfDot4(m->x.w, m->y.w, m->z.w, m->w.w, px, py, pz, 1.0f);
        return true;
    }
    return false;
}
