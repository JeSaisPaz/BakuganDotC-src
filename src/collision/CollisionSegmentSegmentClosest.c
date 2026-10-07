// bdc 0x089e8b70 CollisionSegmentSegmentClosest
#include "bdc.h"

/* Closest points between two segments `a` and `b` (`SegmentShape` layout: origin `+0x10`,
   vector `+0x20`; Ericson's `ClosestPtSegmentSegment`, with the degenerate cases): writes the
   parameters to `*s`/`*t` and the points to `pa`/`pb`, and returns the squared distance between
   the points (x, y, z). Both segments degenerate (squared length <= 0.0001): `s = t = 0` and the
   points are the origins. Parallel segments (zero denominator) take `s = 0.5`. Clamps are
   `vmin.s` against the bank's S733 (1.0f) then `vmax.s` against S713 (0.0f). */

/* vmin.s against S733 (1.0f), then vmax.s against S713 (0.0f). A NaN: +NaN loses vmin and gives 1,
   -NaN wins vmin and then loses vmax, giving 0 (semantics table: vmin/vmax). */
static inline float SegSegClamp01(float q)
{
  if (q != q)
    return __builtin_signbit(q) ? 0.0f : 1.0f;
  return q <= 0.0f ? 0.0f : q > 1.0f ? 1.0f : q;
}

float CollisionSegmentSegmentClosest(const void *a, const void *b, float *s, float *t, ScePspFVector4 *pa, ScePspFVector4 *pb)
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

  /* d = a.start - b.start; aa = |a.dir|^2; bb = |b.dir|^2; bd = b.dir.d */
  d[0] = sa->start[0] - sb->start[0];
  d[1] = sa->start[1] - sb->start[1];
  d[2] = sa->start[2] - sb->start[2];
  aa = sa->dir[0] * sa->dir[0] + sa->dir[1] * sa->dir[1] + sa->dir[2] * sa->dir[2];
  bb = sb->dir[0] * sb->dir[0] + sb->dir[1] * sb->dir[1] + sb->dir[2] * sb->dir[2];
  bd = sb->dir[0] * d[0] + sb->dir[1] * d[1] + sb->dir[2] * d[2];

  if (aa <= 0.0001f && bb <= 0.0001f) {
    /* both degenerate: the origins (all four lanes copied) */
    *t = 0.0f;
    *s = 0.0f;
    pa->x = sa->start[0];
    pa->y = sa->start[1];
    pa->z = sa->start[2];
    pa->w = sa->start[3];
    pb->x = sb->start[0];
    pb->y = sb->start[1];
    pb->z = sb->start[2];
    pb->w = sb->start[3];
    dx = pa->x - pb->x;
    dy = pa->y - pb->y;
    dz = pa->z - pb->z;
    return dx * dx + dy * dy + dz * dz;
  }

  if (aa <= 0.0001f) {
    /* a degenerate: s = 0, t = clamp(bd / bb) */
    *s = 0.0f;
    tN = bd / bb;
    *t = tN;
    *t = SegSegClamp01(tN);
  } else {
    /* ad = a.dir.d */
    ad = sa->dir[0] * d[0] + sa->dir[1] * d[1] + sa->dir[2] * d[2];
    if (bb <= 0.0001f) {
      /* b degenerate: t = 0, s = clamp(-ad / aa) */
      *t = 0.0f;
      *s = SegSegClamp01(-ad / aa);
    } else {
      /* ab = a.dir.b.dir */
      ab = sa->dir[0] * sb->dir[0] + sa->dir[1] * sb->dir[1] + sa->dir[2] * sb->dir[2];
      denom = aa * bb - ab * ab;
      if (denom == 0.0f) {
        sN = 0.5f;
      } else {
        sN = SegSegClamp01((ab * bd - ad * bb) / denom);
      }
      *s = sN;
      tN = (ab * sN + bd) / bb;
      *t = tN;
      if (tN < 0.0f) {
        *t = 0.0f;
        *s = SegSegClamp01(-ad / aa);
      } else if (!(*t <= 1.0f)) {
        *t = 1.0f;
        *s = SegSegClamp01((ab - ad) / aa);
      }
    }
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
