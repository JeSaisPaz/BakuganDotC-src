// bdc 0x089ea030 CollisionAabbSphereContact
#include "bdc.h"

/* If a sphere query (`CollisionSphereQuery`) overlaps the AABB (`CollisionAabbOverlapSphere`),
   writes the point on the sphere surface toward the box centre (`CollisionAabbCenter`) to `out`:
   `center + normalize(boxCentre - center) * radius`; a zero-length direction gives scale 0 (`vcmovt`
   of the bank zero S713), i.e. the sphere centre. `out.w` is 0.0f (S713). Returns the overlap result. */

bool CollisionAabbSphereContact(const float *aabb, const void *sphere, ScePspFVector4 *out)
{
    const CollisionSphereQuery *s = sphere;
    const ScePspFVector4 *boxCentre;
    float radius;
    float lenSq;
    float inv;
    float scale;
    float x;
    float y;
    float z;
    bool hit;

    hit = CollisionAabbOverlapSphere(aabb, sphere);
    if (hit) {
        boxCentre = CollisionAabbCenter((const ScePspFVector4 *)aabb);
        /* vsub.t: lane 3 keeps boxCentre->w (0.0f) */
        out->x = VfSub(boxCentre->x, s->center.x);
        out->y = VfSub(boxCentre->y, s->center.y);
        out->z = VfSub(boxCentre->z, s->center.z);
        out->w = boxCentre->w;
        radius = s->radius;
        x = out->x;
        y = out->y;
        z = out->z;
        lenSq = VfDot3(x, y, z, x, y, z);
        inv = VfRsq(lenSq);
        if (VfIsZero(lenSq)) {
            inv = 0.0f; /* vcmovt.s from S713 */
        }
        scale = VfMul(inv, radius);
        /* vscl.t into C710: lane 3 is S713 (0.0f) */
        out->x = VfMul(x, scale);
        out->y = VfMul(y, scale);
        out->z = VfMul(z, scale);
        out->w = 0.0f;
        x = VfAdd(out->x, s->center.x);
        y = VfAdd(out->y, s->center.y);
        z = VfAdd(out->z, s->center.z);
        out->x = x;
        out->y = y;
        out->z = z;
    }
    return hit;
}
