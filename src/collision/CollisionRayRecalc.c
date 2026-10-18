// bdc 0x08a2a184 CollisionRayRecalc
#include "bdc.h"

/* Recomputes the derived data of a ray shape (vtable `0x08af5504` entry 9, offset `+0x4c`): the
   per-axis inverse direction `invDir` = 1 / `dir`, guarded per component against zero (a zero
   component gives 0). `invDir.w` is set to 0. */

void CollisionRayRecalc(CollisionRayShape *ray)

{
  ScePspFVector4 dir = ray->dir;
  ScePspFVector4 inv;

  inv.x = VfRcp(dir.x);
  if (VfIsZero(dir.x)) inv.x = 0.0f;
  inv.y = VfRcp(dir.y);
  if (VfIsZero(dir.y)) inv.y = 0.0f;
  inv.z = VfRcp(dir.z);
  if (VfIsZero(dir.z)) inv.z = 0.0f;
  inv.w = 0.0f;
  ray->invDir = inv;
}
