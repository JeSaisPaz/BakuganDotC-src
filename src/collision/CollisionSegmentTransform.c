// bdc 0x089e8f18 CollisionSegmentTransform
#include "bdc.h"

/* Transform method of the segment shape (vtable `0x08af5564` entry 6): writes into `out` the
   segment transformed by `mtx` (start `+0x10` as a point with w = 1, extent vector `+0x20` as a
   vector) and copies the type word `+0x0`. */

void CollisionSegmentTransform(void *segment, const ScePspFMatrix4 *mtx, void *out)

{
  const SegmentShape *src = segment;
  SegmentShape *dst = out;
  float w;
  float vx;
  float vy;
  float vz;
  ScePspFVector4 t;
  const ScePspFVector4 zero = {0.0f, 0.0f, 0.0f, 0.0f};

  /* vtfm4.q with the start's lane 3 replaced by 1.0f (vfim.s S203, 0x3c00) */
  vx = src->start[0];
  vy = src->start[1];
  vz = src->start[2];
  VfDot4x4(&t.x, &mtx->x.x, &mtx->y.x, &mtx->z.x, &mtx->w.x, vx, vy, vz, 1.0f);
  w = t.w;
  dst->start[0] = t.x;
  dst->start[1] = t.y;
  dst->start[2] = t.z;
  dst->start[3] = w;
  /* vtfm3.t writes lanes 0-2 of C000; the sv.q stores lane 3 still holding the transformed w */
  vx = src->dir[0];
  vy = src->dir[1];
  vz = src->dir[2];
  VfDot4x4(&t.x, &mtx->x.x, &mtx->y.x, &mtx->z.x, &zero.x, vx, vy, vz, 0.0f); /* VfDot3: 4th term 0 x 0 */
  dst->dir[0] = t.x;
  dst->dir[1] = t.y;
  dst->dir[2] = t.z;
  dst->dir[3] = w;
  dst->type = src->type;
}
