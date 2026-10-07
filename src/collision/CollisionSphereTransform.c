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
  out->center[0] = mtx->x.x * x + mtx->y.x * y + mtx->z.x * z + mtx->w.x;
  out->center[1] = mtx->x.y * x + mtx->y.y * y + mtx->z.y * z + mtx->w.y;
  out->center[2] = mtx->x.z * x + mtx->y.z * y + mtx->z.z * z + mtx->w.z;
  r = sphere->radius;
  out->radius = r;
  out->radiusSq = r * r;
  out->type = sphere->type;
}
