// bdc 0x089e8864 CollisionSegmentClosestPoint
#include "bdc.h"

/* Closest point on the segment `seg` (origin `+0x10`, vector `+0x20`) to `p`: stores the clamped
   parameter (0..1) in `*t` and the point in `out` (`w` copied from the origin's `w`); returns the
   squared distance from `p` to that point. */

float CollisionSegmentClosestPoint(const void *seg, const ScePspFVector4 *p, float *t, ScePspFVector4 *out)

{
  const SegmentShape *s = seg;
  float d[3];
  float aa;
  float ab;
  float q;
  float param;
  float ox;
  float oy;
  float oz;

  d[0] = s->start[0] - p->x;
  d[1] = s->start[1] - p->y;
  d[2] = s->start[2] - p->z;
  aa = s->dir[0] * s->dir[0] + s->dir[1] * s->dir[1] + s->dir[2] * s->dir[2];
  ab = s->dir[0] * d[0] + s->dir[1] * d[1] + s->dir[2] * d[2];
  q = -ab / aa;
  /* vmin.s against S733 (1.0f), then vmax.s against S713 (0.0f). A NaN (dir of length 0): +NaN
     loses vmin and gives 1, -NaN wins vmin and then loses vmax, giving 0 (VfpuLift: vmin/vmax). */
  if (q != q)
    param = __builtin_signbit(q) ? 0.0f : 1.0f;
  else
    param = q <= 0.0f ? 0.0f : q > 1.0f ? 1.0f : q;
  *t = param;
  /* vmul.t by (param, param, param), vadd.t to the origin; the sv.q stores the origin's w in lane 3 */
  out->x = s->start[0] + s->dir[0] * param;
  out->y = s->start[1] + s->dir[1] * param;
  out->z = s->start[2] + s->dir[2] * param;
  out->w = s->start[3];
  ox = out->x - p->x;
  oy = out->y - p->y;
  oz = out->z - p->z;
  return ox * ox + oy * oy + oz * oz;
}
