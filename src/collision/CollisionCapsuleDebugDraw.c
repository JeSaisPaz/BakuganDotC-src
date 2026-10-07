// bdc 0x089e980c CollisionCapsuleDebugDraw
#include "bdc.h"

/* Debug-draw method of the capsule shape (type-4 slot 6): adds the capsule wireframe
   (`CollisionDebugAddCapsule`). */

void CollisionCapsuleDebugDraw(void *capsule, const ScePspFVector4 *colour, const ScePspFMatrix4 *mtx)

{
  CollisionDebugAddCapsule(capsule,colour,mtx,1);
  return;
}

