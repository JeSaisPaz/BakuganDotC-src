// bdc 0x089f0184 CollisionDebugAddSphereRings
#include "bdc.h"

/* Adds the three ring primitives of a sphere (`CollisionDebugAddRingXY`,
   `CollisionDebugAddRingYZ`, `CollisionDebugAddRingXZ`) and sets their lifetime `+0x90` to
   `frames`. */

void CollisionDebugAddSphereRings(float r, const ScePspFVector4 *centre, const ScePspFVector4 *colour, const ScePspFMatrix4 *mtx, s32 frames)

{
  CollisionDebugPrim *prim;
  
  prim = CollisionDebugAddRingXY(r,centre,colour,mtx);
  prim->frames = frames;
  prim = CollisionDebugAddRingYZ(r,centre,colour,mtx);
  prim->frames = frames;
  prim = CollisionDebugAddRingXZ(r,centre,colour,mtx);
  prim->frames = frames;
  return;
}

