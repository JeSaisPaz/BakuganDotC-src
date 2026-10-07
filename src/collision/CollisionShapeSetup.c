// bdc 0x089e530c CollisionShapeSetup
#include "bdc.h"

/* Installs a shape descriptor into a shape block (`block = collider + 0x30`): stores the shape type
   `desc[0]` in `shapeType` and `desc` itself in `desc`, sets `installed = 1` and, for types 1, 2, 3,
   4 and 6, writes the shape header (`type`, vtable: ray, segment, sphere, capsule, box) at the start
   of the block and saves the block pointer in `shape`. Type 4 additionally writes the embedded type-2
   segment sub-shape header (`subType`, `subVtbl`); type 6 clears `boxInvValid`. Types 0, 5, 7, 8 and
   anything above only record the type. */

void CollisionShapeSetup(u32 *block, u32 *desc)
{
  CollisionShapeBlock *b = (CollisionShapeBlock *)block;
  CollisionShapeBlock *shape;

  b->shapeType = (s32)desc[0];
  b->desc = desc;
  b->installed = 1;
  switch ((u32)b->shapeType) {
  case 0:
    return;
  case 1:
    shape = NULL;
    if (b != NULL) {
      b->vtbl = g_collisionRayVtbl;
      b->type = 1;
      shape = b;
    }
    b->shape = shape;
    return;
  case 2:
    shape = NULL;
    if (b != NULL) {
      b->vtbl = g_collisionSegmentVtbl;
      b->type = 2;
      shape = b;
    }
    b->shape = shape;
    return;
  case 3:
    shape = NULL;
    if (b != NULL) {
      b->vtbl = g_collisionSphereVtbl;
      b->type = 3;
      shape = b;
    }
    b->shape = shape;
    return;
  case 4:
    shape = NULL;
    if (b != NULL) {
      b->vtbl = g_collisionCapsuleVtbl;
      b->subVtbl = g_collisionSegmentVtbl;
      b->subType = 2;
      b->type = 4;
      shape = b;
    }
    b->shape = shape;
    return;
  case 5:
    return;
  case 6:
    shape = NULL;
    if (b != NULL) {
      b->vtbl = g_collisionBoxVtbl;
      b->boxInvValid = 0;
      b->type = 6;
      shape = b;
    }
    b->shape = shape;
    return;
  default:
    return;
  }
}
