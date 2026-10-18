// bdc 0x089e9328 CollisionSphereTransform
#include "bdc.h"

/* Transform method of the sphere shape (vtable `0x08af55c4` entry 6): writes into `out` the centre
   transformed by `mtx` (as the point (centre, 1)), the radius (not scaled) and its square, and copies
   the type word. */

void CollisionSphereTransform(const CollisionSphere *sphere, const ScePspFMatrix4 *mtx,
                              CollisionSphere *out)
{
  float x = sphere->center[0];
  float y = sphere->center[1];
  float z = sphere->center[2];
  float r;

  /* vtfm4.q with the w lane set to 1.0f (vfim); the result's w lane lands on radiusSq and is
     overwritten below, so it is not computed. */
  out->center[0] = VfDot4(mtx->x.x, mtx->y.x, mtx->z.x, mtx->w.x, x, y, z, 1.0f);
  out->center[1] = VfDot4(mtx->x.y, mtx->y.y, mtx->z.y, mtx->w.y, x, y, z, 1.0f);
  out->center[2] = VfDot4(mtx->x.z, mtx->y.z, mtx->z.z, mtx->w.z, x, y, z, 1.0f);
  r = sphere->radius;
  out->radius = r;
  out->radiusSq = r * r;
  out->type = sphere->type;
}
