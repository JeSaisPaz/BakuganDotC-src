// bdc 0x0881a444 CollisionColliderInitMesh
#include "bdc.h"

/* Mesh variant of `CollisionColliderInit`: makes the collider's shape block (`+0x30`) a mesh
   shape (kind 8, mesh `mesh`, via `CollisionShapeSetMesh` → `CollisionMeshLoad`; the mesh is
   prepared unless `layer == 8`), clears the hit result (`CollisionColliderClearHit`) and stores
   `layer` (`+0x134`), the owning object `owner` (`+0x138`, or NULL) and `flags` (`+0x130`). */

void CollisionColliderInitMesh(CoreNode *collider_, void *mesh, u32 layer, void *owner, u32 flags)

{
  CollisionCollider *collider = (CollisionCollider *)collider_;

  if (layer == 8) {
    CollisionShapeSetMesh(collider->shapeBlock, mesh, false);
  }
  else {
    CollisionShapeSetMesh(collider->shapeBlock, mesh, true);
  }
  CollisionColliderClearHit(collider);
  collider->layer = layer;
  collider->owner = owner;
  collider->flags = flags;
}
