// bdc 0x089e869c CollisionRayTransform
#include "bdc.h"

/* Transform method of the ray shape (vtable `0x08af5504` entry 6): writes into `out` the ray
   transformed by `mtx` (origin `+0x10` as a point with w = 1, direction `+0x20` as a vector),
   recomputes its zero-safe per-axis inverse direction `+0x30` (0 for a zero component, w = 0) and
   copies the type word `+0x0`. The direction's w lane keeps the transformed origin's w. */

void CollisionRayTransform(const CollisionRayShape *ray, const ScePspFMatrix4 *mtx, CollisionRayShape *out)

{
  ScePspFVector4 o = ray->origin;
  ScePspFVector4 r;
  ScePspFVector4 d;
  ScePspFVector4 inv;

  o.w = 1.0f;
  r.x = VfDot4(mtx->x.x, mtx->y.x, mtx->z.x, mtx->w.x, o.x, o.y, o.z, o.w);
  r.y = VfDot4(mtx->x.y, mtx->y.y, mtx->z.y, mtx->w.y, o.x, o.y, o.z, o.w);
  r.z = VfDot4(mtx->x.z, mtx->y.z, mtx->z.z, mtx->w.z, o.x, o.y, o.z, o.w);
  r.w = VfDot4(mtx->x.w, mtx->y.w, mtx->z.w, mtx->w.w, o.x, o.y, o.z, o.w);
  out->origin = r;

  d = ray->dir;
  /* vtfm3 writes x/y/z only; the w lane still holds the transformed origin's w */
  r.x = VfDot3(mtx->x.x, mtx->y.x, mtx->z.x, d.x, d.y, d.z);
  r.y = VfDot3(mtx->x.y, mtx->y.y, mtx->z.y, d.x, d.y, d.z);
  r.z = VfDot3(mtx->x.z, mtx->y.z, mtx->z.z, d.x, d.y, d.z);
  out->dir = r;

  /* bank constant S713 = 0.0f: zero-component substitute and inv.w */
  d = out->dir;
  inv.x = VfIsZero(d.x) ? 0.0f : VfRcp(d.x);
  inv.y = VfIsZero(d.y) ? 0.0f : VfRcp(d.y);
  inv.z = VfIsZero(d.z) ? 0.0f : VfRcp(d.z);
  inv.w = 0.0f;
  out->invDir = inv;

  out->type = ray->type;
}
