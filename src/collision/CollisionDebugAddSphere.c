// bdc 0x089f020c CollisionDebugAddSphere
#include "bdc.h"

/* Adds the wireframe of a sphere shape (radius `+0x20`, centre `+0x10`) for one frame
   (`CollisionDebugAddSphereRings`). */

void CollisionDebugAddSphere(const CollisionSphereQuery *sphere, const ScePspFVector4 *colour, const ScePspFMatrix4 *mtx)

{
  CollisionDebugAddSphereRings(sphere->radius,&sphere->center,colour,mtx,1);
  return;
}
