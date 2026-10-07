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
        out->x = boxCentre->x - s->center.x;
        out->y = boxCentre->y - s->center.y;
        out->z = boxCentre->z - s->center.z;
        out->w = boxCentre->w;
        radius = s->radius;
        x = out->x;
        y = out->y;
        z = out->z;
        lenSq = x * x + y * y + z * z;
        inv = VfRsq(lenSq);
        if (lenSq == 0.0f) {
            inv = 0.0f; /* vcmovt.s from S713 */
        }
        scale = inv * radius;
        /* vscl.t into C710: lane 3 is S713 (0.0f) */
        out->x = x * scale;
        out->y = y * scale;
        out->z = z * scale;
        out->w = 0.0f;
        x = out->x + s->center.x;
        y = out->y + s->center.y;
        z = out->z + s->center.z;
        out->x = x;
        out->y = y;
        out->z = z;
    }
    return hit;
}
