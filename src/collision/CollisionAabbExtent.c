// bdc 0x089ea1f4 CollisionAabbExtent
#include "bdc.h"

/* Writes `max - min` (xyz) of an AABB (min at aabb[0..3], max at aabb[4..7]) to `out`, w copied from max.w, and returns `out`. */

ScePspFVector4 *CollisionAabbExtent(const float *aabb, ScePspFVector4 *out)
{
  const float *max = aabb + 4;
  ScePspFVector4 r;

  r.x = max[0] - aabb[0];
  r.y = max[1] - aabb[1];
  r.z = max[2] - aabb[2];
  r.w = max[3];
  *out = r;
  return out;
}
