// bdc 0x089e9238 CollisionSphereVsCapsule
#include "bdc.h"

/* Sphere shape method for capsule queries (type-3 slot 3): closest point on the capsule axis to the
   centre; returns true when the squared distance is within the summed radii squared; `out` = midpoint
   between the centre (4 lanes, w = radiusSq) and that point. */

bool CollisionSphereVsCapsule(const void *sphere, const void *capsule, ScePspFVector4 *out)

{
  const CollisionSphere *sph = sphere;
  const CollisionCapsule *cap = capsule;
  ScePspFVector4 *p = &g_collisionClosestPoints[0];
  const ScePspFVector4 *c = (const ScePspFVector4 *)sph->center;
  float distSq;
  float r;
  ScePspFVector4 mid;

  distSq = CollisionSegmentClosestPoint(cap->segmentHead, c, g_collisionClosestParams, p);
  r = sph->radius + cap->radius;
  mid.x = p->x + (c->x - p->x) * 0.5f;
  mid.y = p->y + (c->y - p->y) * 0.5f;
  mid.z = p->z + (c->z - p->z) * 0.5f;
  mid.w = p->w + (c->w - p->w) * 0.5f;
  *out = mid;
  return distSq <= r * r;
}
