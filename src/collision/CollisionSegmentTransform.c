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

  /* vtfm4.q with the start's lane 3 replaced by 1.0f (vfim.s S203, 0x3c00) */
  vx = src->start[0];
  vy = src->start[1];
  vz = src->start[2];
  w = VfDot4(mtx->x.w, mtx->y.w, mtx->z.w, mtx->w.w, vx, vy, vz, 1.0f);
  dst->start[0] = VfDot4(mtx->x.x, mtx->y.x, mtx->z.x, mtx->w.x, vx, vy, vz, 1.0f);
  dst->start[1] = VfDot4(mtx->x.y, mtx->y.y, mtx->z.y, mtx->w.y, vx, vy, vz, 1.0f);
  dst->start[2] = VfDot4(mtx->x.z, mtx->y.z, mtx->z.z, mtx->w.z, vx, vy, vz, 1.0f);
  dst->start[3] = w;
  /* vtfm3.t writes lanes 0-2 of C000; the sv.q stores lane 3 still holding the transformed w */
  vx = src->dir[0];
  vy = src->dir[1];
  vz = src->dir[2];
  dst->dir[0] = VfDot3(mtx->x.x, mtx->y.x, mtx->z.x, vx, vy, vz);
  dst->dir[1] = VfDot3(mtx->x.y, mtx->y.y, mtx->z.y, vx, vy, vz);
  dst->dir[2] = VfDot3(mtx->x.z, mtx->y.z, mtx->z.z, vx, vy, vz);
  dst->dir[3] = w;
  dst->type = src->type;
}
