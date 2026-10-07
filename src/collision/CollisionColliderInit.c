// bdc 0x0881a4e0 CollisionColliderInit
#include "bdc.h"

/* Configures a collider built by `CollisionColliderCtor`: sets up the shape block at `+0x30` from
   the shape descriptor `shape` (`CollisionShapeSetup`), clears the hit result
   (`CollisionColliderClearHit`), then stores `layer` at `+0x134` (the layer tested by
   `CollisionRaycast`), the owning object `owner` at `+0x138` (callers pass the actor/unit/gimmick
   that holds the collider, or NULL) and `flags` at `+0x130` (raycast skips colliders with bit 1 or
   2 set). */

void CollisionColliderInit(CoreNode *collider_, const u32 *shape, u32 layer, void *owner, u32 flags)

{
  CollisionCollider *collider = (CollisionCollider *)collider_;

  CollisionShapeSetup((u32 *)collider->shapeBlock, (u32 *)shape);
  CollisionColliderClearHit(collider);
  collider->layer = layer;
  collider->owner = owner;
  collider->flags = flags;
}
