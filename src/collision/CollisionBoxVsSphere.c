// bdc 0x089ea5a8 CollisionBoxVsSphere
#include "bdc.h"

/* Box shape method for sphere queries (type-6 vtable slot 2): caches the inverse box transform
   (`invTransform`, flag `invValid`; transposed rotation with the negated rotated translation),
   transforms the query into box space through its vtable (slot 6) into the static sphere shape
   `g_collisionBoxSphereQueryShape`, runs `CollisionAabbSphereContact` against the box extents and, on a hit, transforms the point in
   `out` back to world space (`(out.xyz, 1) * transform`). Returns whether it hit. */

bool CollisionBoxVsSphere(CollisionBox *self, void *sphere, ScePspFVector4 *out)
{
    const VtblEntry *vtbl;
    const ScePspFMatrix4 *m;
    ScePspFMatrix4 *inv;
    float tx, ty, tz;
    float px, py, pz;

    if (g_collisionBoxSphereQueryInit == 0) {
        g_collisionBoxSphereQueryInit = 1;
        g_collisionBoxSphereQueryShape.vtbl = g_collisionSphereVtbl;
        g_collisionBoxSphereQueryShape.type = 3;
    }
    if (self->invValid == 0) {
        m = &self->transform;
        inv = &self->invTransform;
        /* rotated translation: dot3(axis_i, translation) */
        tx = m->x.x * m->w.x + m->x.y * m->w.y + m->x.z * m->w.z;
        ty = m->y.x * m->w.x + m->y.y * m->w.y + m->y.z * m->w.z;
        tz = m->z.x * m->w.x + m->z.y * m->w.y + m->z.z * m->w.z;
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
    vtbl = ((const CollisionBox *)sphere)->vtbl + 6;
    ((void (*)(void *, const ScePspFMatrix4 *, CollisionSphere *))vtbl->fn)(
        (char *)sphere + vtbl->delta, &self->invTransform, &g_collisionBoxSphereQueryShape);
    if (CollisionAabbSphereContact(&(self->aabbMin).x, &g_collisionBoxSphereQueryShape, out)) {
        m = &self->transform;
        px = out->x;
        py = out->y;
        pz = out->z;
        out->x = m->x.x * px + m->y.x * py + m->z.x * pz + m->w.x;
        out->y = m->x.y * px + m->y.y * py + m->z.y * pz + m->w.y;
        out->z = m->x.z * px + m->y.z * py + m->z.z * pz + m->w.z;
        out->w = m->x.w * px + m->y.w * py + m->z.w * pz + m->w.w;
        return true;
    }
    return false;
}
