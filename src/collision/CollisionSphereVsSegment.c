// bdc 0x089e912c CollisionSphereVsSegment
#include "bdc.h"

/* Sphere shape method for segment queries (type-3 slot 1): closest point of the segment to the
   centre (`CollisionSegmentClosestPoint`) into `out`; hit when the returned squared distance is below the squared radius (`+0x20`, read after the
   call). Returns that flag. */

bool CollisionSphereVsSegment(const void *sphere, const void *seg, ScePspFVector4 *out)

{
  const CollisionSphere *sph = sphere;
  float t;
  float d;

  d = CollisionSegmentClosestPoint(seg, (const ScePspFVector4 *)sph->center, &t, out);
  return d < sph->radius * sph->radius;
}
