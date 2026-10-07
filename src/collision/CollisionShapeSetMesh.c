// bdc 0x089e52e0 CollisionShapeSetMesh
#include "bdc.h"

/* Makes a shape block a triangle-mesh shape: `shapeType` 8, the collision file in `desc`, then
   `CollisionMeshLoad` (`keepWorldVerts` allocates per-part transformed data). */

void CollisionShapeSetMesh(void *block, void *file, bool keepWorldVerts)

{
  CollisionShapeBlock *b = (CollisionShapeBlock *)block;

  b->shapeType = 8;
  b->desc = file;
  CollisionMeshLoad(block,keepWorldVerts);
  return;
}
