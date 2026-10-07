// bdc 0x089e54c8 CollisionInitQueryShapes
#include "bdc.h"

/* Installs the five static query shape descriptors (`0x08b002c0` type 1 ray, `0x08b00400` type 2
   segment, `0x08b00530` type 3 sphere, `0x08b00660` type 4 swept sphere, `0x08b00ad0` type 6 box)
   into their two shape blocks each with `CollisionShapeSetup`, clearing each block's
   `installed` byte. The first swept-sphere block is the one at `g_collisionSweptSphereDesc + 0x50`
   (`shapeBlock`). Called by `BtlMainTaskCtor` and `GameFieldCtor`. */

void CollisionInitQueryShapes(void)
{
  CollisionShapeBlock *swept = (CollisionShapeBlock *)g_collisionSweptSphereDesc.shapeBlock;

  CollisionShapeSetup((u32 *)&g_collisionRayBlock, (u32 *)&g_collisionRayDesc);
  g_collisionRayBlock.installed = 0;
  CollisionShapeSetup((u32 *)&g_collisionRayBlock2, (u32 *)&g_collisionRayDesc);
  g_collisionRayBlock2.installed = 0;

  CollisionShapeSetup((u32 *)&g_collisionSegmentBlock, (u32 *)&g_collisionSegmentDesc);
  g_collisionSegmentBlock.installed = 0;
  CollisionShapeSetup((u32 *)&g_collisionSegmentBlock2, (u32 *)&g_collisionSegmentDesc);
  g_collisionSegmentBlock2.installed = 0;

  CollisionShapeSetup((u32 *)&g_collisionSphereBlock, (u32 *)&g_btlBakuganSphereQuery);
  g_collisionSphereBlock.installed = 0;
  CollisionShapeSetup((u32 *)&g_collisionSphereBlock2, (u32 *)&g_btlBakuganSphereQuery);
  g_collisionSphereBlock2.installed = 0;

  CollisionShapeSetup((u32 *)swept, (u32 *)&g_collisionSweptSphereDesc);
  swept->installed = 0;
  CollisionShapeSetup((u32 *)&g_collisionSweptSphereBlock2, (u32 *)&g_collisionSweptSphereDesc);
  g_collisionSweptSphereBlock2.installed = 0;

  CollisionShapeSetup((u32 *)&g_collisionBoxBlock, (u32 *)&g_collisionBoxDesc);
  g_collisionBoxBlock.installed = 0;
  CollisionShapeSetup((u32 *)&g_collisionBoxBlock2, (u32 *)&g_collisionBoxDesc);
  g_collisionBoxBlock2.installed = 0;
}
