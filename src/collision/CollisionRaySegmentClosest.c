// bdc 0x089e8934 CollisionRaySegmentClosest
#include "bdc.h"

/* Closest points between a ray `a` and a segment `b` (`SegmentShape` layout: origin `+0x10`,
   vector `+0x20`; Ericson's two-line solution without the degenerate-length cases): writes the
   parameters to `*s`/`*t` and the points to `pa`/`pb`, and returns the squared distance between
   the points (x, y, z). Parallel lines (zero denominator) take `s = 0.5` unclamped; otherwise `s`
   is clamped to [0, 1] (`vmin.s` against the bank's S733, then `vmax.s` against S713). `t < 0`
   sets `t = 0` and recomputes `s`; `t > |b.dir|^2` recomputes `s` but leaves `*t` unwritten (see
   Notes). */

/* vmin.s against S733 (1.0f), then vmax.s against S713 (0.0f). A NaN: +NaN loses vmin and gives 1,
   -NaN wins vmin and then loses vmax, giving 0 (semantics table: vmin/vmax). */
static inline float RaySegClamp01(float q)
{
  if (q != q)
    return __builtin_signbit(q) ? 0.0f : 1.0f;
  return q <= 0.0f ? 0.0f : q > 1.0f ? 1.0f : q;
}

float CollisionRaySegmentClosest(const void *a, const void *b, float *s, float *t, ScePspFVector4 *pa, ScePspFVector4 *pb)
{
  const SegmentShape *sa = a;
  const SegmentShape *sb = b;
  float d[3];
  float aa;
  float bb;
  float bd;
  float ad;
  float ab;
  float denom;
  float sN;
  float tN;
  float k;
  float dx;
  float dy;
  float dz;

  /* d = a.start - b.start; aa = |a.dir|^2; bb = |b.dir|^2; bd = b.dir.d; ad = a.dir.d;
     ab = a.dir.b.dir */
  d[0] = sa->start[0] - sb->start[0];
  d[1] = sa->start[1] - sb->start[1];
  d[2] = sa->start[2] - sb->start[2];
  aa = sa->dir[0] * sa->dir[0] + sa->dir[1] * sa->dir[1] + sa->dir[2] * sa->dir[2];
  bb = sb->dir[0] * sb->dir[0] + sb->dir[1] * sb->dir[1] + sb->dir[2] * sb->dir[2];
  bd = sb->dir[0] * d[0] + sb->dir[1] * d[1] + sb->dir[2] * d[2];
  ad = sa->dir[0] * d[0] + sa->dir[1] * d[1] + sa->dir[2] * d[2];
  ab = sa->dir[0] * sb->dir[0] + sa->dir[1] * sb->dir[1] + sa->dir[2] * sb->dir[2];

  denom = aa * bb - ab * ab;
  if (denom == 0.0f) {
    sN = 0.5f;
  } else {
    sN = RaySegClamp01((ab * bd - ad * bb) / denom);
  }
  *s = sN;
  tN = ab * sN + bd;
  if (tN < 0.0f) {
    *t = 0.0f;
    *s = RaySegClamp01(-ad / aa);
  } else if (bb < tN) {
    /* *t is not written here */
    *s = RaySegClamp01((ab - ad) / aa);
  } else {
    *t = tN / bb;
  }

  /* pa = a.start + a.dir * s (w from a.start) */
  k = *s;
  pa->x = sa->start[0] + sa->dir[0] * k;
  pa->y = sa->start[1] + sa->dir[1] * k;
  pa->z = sa->start[2] + sa->dir[2] * k;
  pa->w = sa->start[3];

  /* pb = b.start + b.dir * t (w from b.start); return |pa - pb|^2 */
  k = *t;
  pb->x = sb->start[0] + sb->dir[0] * k;
  pb->y = sb->start[1] + sb->dir[1] * k;
  pb->z = sb->start[2] + sb->dir[2] * k;
  pb->w = sb->start[3];
  dx = pa->x - pb->x;
  dy = pa->y - pb->y;
  dz = pa->z - pb->z;
  return dx * dx + dy * dy + dz * dz;
}
