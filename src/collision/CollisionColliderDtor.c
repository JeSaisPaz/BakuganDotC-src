// bdc 0x0881a3b0 CollisionColliderDtor
#include "bdc.h"

/* Destructor of a collider node (`CollisionColliderCtor`, vtable `0x08af1684`): restores the
   vtable, releases the shape block `+0x30` (`CollisionShapeBlockDtor(block, 2)`), runs `CoreNodeDtor` and
   frees the node when `flags & 1`. */

void CollisionColliderDtor(CoreNode *collider, u32 flags)
{
  CollisionCollider *c = (CollisionCollider *)collider;

  if (collider != (CoreNode *)0x0) {
    collider->vtable = &g_collisionColliderVtable;
    CollisionShapeBlockDtor(c->shapeBlock, 2);
    CoreNodeDtor(collider, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(collider, (char *)0x0, 0);
      MemUnlock();
    }
  }
}
