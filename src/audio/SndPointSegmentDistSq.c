// bdc 0x089bfbd8 SndPointSegmentDistSq
#include "bdc.h"

/* Squared distance from the point `point` to the segment `a`..`b` (all three-float vectors), and
   optionally (when `outT` is non-NULL) the segment parameter `t` in [0,1] of the closest point. It
   builds a temporary segment shape (start `a`, direction `b - a`) on the stack and delegates to
   `SegmentDistSqToPoint` / `SegmentClosestParam`. */

float SndPointSegmentDistSq(const float *point, const float *a, const float *b, float *outT)
{
  float p[4] __attribute__((aligned(16)));
  float va[4] __attribute__((aligned(16)));
  float vb[4] __attribute__((aligned(16)));
  float tmp[4] __attribute__((aligned(16)));
  SegmentShape seg __attribute__((aligned(16)));
  float distSq;

  p[0] = point[0];
  p[1] = point[1];
  p[2] = point[2];
  p[3] = 0.0f;
  va[0] = a[0];
  va[1] = a[1];
  va[2] = a[2];
  va[3] = 0.0f;
  vb[0] = b[0];
  vb[1] = b[1];
  vb[2] = b[2];
  vb[3] = 0.0f;
  seg.info = g_collisionSegmentVtbl;
  seg.type = 2;
  /* dir = b - a over x, y, z (vsub.t keeps lane w of b, 0), staged through tmp; start = a. */
  tmp[0] = vb[0] - va[0];
  tmp[1] = vb[1] - va[1];
  tmp[2] = vb[2] - va[2];
  tmp[3] = vb[3];
  seg.dir[0] = tmp[0];
  seg.dir[1] = tmp[1];
  seg.dir[2] = tmp[2];
  seg.dir[3] = tmp[3];
  seg.start[0] = va[0];
  seg.start[1] = va[1];
  seg.start[2] = va[2];
  seg.start[3] = va[3];
  distSq = SegmentDistSqToPoint(&seg, p);
  if (outT != NULL) {
    *outT = SegmentClosestParam(&seg, p);
  }
  return distSq;
}
