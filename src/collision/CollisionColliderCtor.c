// bdc 0x0881a318 CollisionColliderCtor
#include "bdc.h"

/* Constructor of a collider node (the 0x190-byte objects that actors load from `.ctc` resources,
   e.g. `"fz_VexosBarrier.ctc"` in `ActorStartVexosBarrier`): runs `CoreNodeCtor`, installs
   vtable `0x08af1684` (`+0x20`), initialises the shape block at `+0x30` with
   `CollisionShapeBlockInit` and clears the byte at `+0x180`. `flags & 2` links the collider into
   the group `0x08b00260` (`CoreNodeGroupAppend`), the list that `CollisionRaycast` iterates;
   `(flags & 3) == 3` sets `+0x180 = 1`, otherwise `flags & 1` links it into the second group
   `0x08b00270`. */

CoreNode *CollisionColliderCtor(CoreNode *collider_, u32 flags)

{
  CollisionCollider *collider = (CollisionCollider *)collider_;

  CoreNodeCtor(collider_, (CoreNode *)0x0);
  collider->node.vtable = &g_collisionColliderVtable;
  CollisionShapeBlockInit(collider->shapeBlock);
  collider->byte180 = 0;
  if ((flags & 2) != 0) {
    CoreNodeGroupAppend(collider_, &g_collisionRaycastColliders);
  }
  if ((flags & 3) == 3) {
    collider->byte180 = 1;
  }
  else if ((flags & 1) != 0) {
    CoreNodeGroupAppend(collider_, &g_collisionHitColliders);
  }
  return collider_;
}
