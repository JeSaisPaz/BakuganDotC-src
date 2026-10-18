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

  d[0] = VfSub(s->start[0], p->x);
  d[1] = VfSub(s->start[1], p->y);
  d[2] = VfSub(s->start[2], p->z);
  aa = VfDot3(s->dir[0], s->dir[1], s->dir[2], s->dir[0], s->dir[1], s->dir[2]);
  ab = VfDot3(s->dir[0], s->dir[1], s->dir[2], d[0], d[1], d[2]);
  q = -ab / aa;
  /* vmin.s against S733 (1.0f), then vmax.s against S713 (0.0f). A NaN (dir of length 0): +NaN
     loses vmin and gives 1, -NaN wins vmin and then loses vmax, giving 0 (VfpuLift: vmin/vmax). */
  if (q != q)
    param = __builtin_signbit(q) ? 0.0f : 1.0f;
  else
    param = VfMax(VfMin(q, 1.0f), 0.0f);
  *t = param;
  /* vmul.t by (param, param, param), vadd.t to the origin; the sv.q stores the origin's w in lane 3 */
  out->x = VfAdd(s->start[0], VfMul(s->dir[0], param));
  out->y = VfAdd(s->start[1], VfMul(s->dir[1], param));
  out->z = VfAdd(s->start[2], VfMul(s->dir[2], param));
  out->w = s->start[3];
  ox = VfSub(out->x, p->x);
  oy = VfSub(out->y, p->y);
  oz = VfSub(out->z, p->z);
  return VfDot3(ox, oy, oz, ox, oy, oz);
}
