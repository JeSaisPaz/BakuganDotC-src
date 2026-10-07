// bdc 0x089e936c CollisionSphereDebugDraw
#include "bdc.h"

/* Debug-draw method of the sphere shape (type-3 slot 6): adds three orthogonal rings of its radius
   around its centre (`CollisionDebugAddSphere`) with lifetime 1. */

void CollisionSphereDebugDraw(void *sphere, const ScePspFVector4 *colour, const ScePspFMatrix4 *mtx)

{
  CollisionDebugAddSphere(sphere,colour,mtx);
  return;
}

