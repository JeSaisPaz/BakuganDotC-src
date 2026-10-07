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
  r.x = mtx->x.x * o.x + mtx->y.x * o.y + mtx->z.x * o.z + mtx->w.x * o.w;
  r.y = mtx->x.y * o.x + mtx->y.y * o.y + mtx->z.y * o.z + mtx->w.y * o.w;
  r.z = mtx->x.z * o.x + mtx->y.z * o.y + mtx->z.z * o.z + mtx->w.z * o.w;
  r.w = mtx->x.w * o.x + mtx->y.w * o.y + mtx->z.w * o.z + mtx->w.w * o.w;
  out->origin = r;

  d = ray->dir;
  /* vtfm3 writes x/y/z only; the w lane still holds the transformed origin's w */
  r.x = mtx->x.x * d.x + mtx->y.x * d.y + mtx->z.x * d.z;
  r.y = mtx->x.y * d.x + mtx->y.y * d.y + mtx->z.y * d.z;
  r.z = mtx->x.z * d.x + mtx->y.z * d.y + mtx->z.z * d.z;
  out->dir = r;

  /* bank constant S713 = 0.0f: zero-component substitute and inv.w */
  d = out->dir;
  inv.x = (d.x == 0.0f) ? 0.0f : 1.0f / d.x;
  inv.y = (d.y == 0.0f) ? 0.0f : 1.0f / d.y;
  inv.z = (d.z == 0.0f) ? 0.0f : 1.0f / d.z;
  inv.w = 0.0f;
  out->invDir = inv;

  out->type = ray->type;
}
