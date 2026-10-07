// bdc 0x089e6640 CollisionQueryShapesStaticInit
#include "bdc.h"

/* Static constructor of the collision query shapes: sets the five descriptors (types 1, 2, 3, 4
   with a type-2 sub-shape, 6) and their vtables (`0x08af5504`, `0x08af5564`, `0x08af55c4`,
   `0x08af5624`, `0x08af5684`), initialises the ten shape blocks (`CollisionShapeBlockInit`) and
   registers each for destruction (`CxxRegisterGlobalObject`). */

void CollisionQueryShapesStaticInit(void)

{
  g_collisionRayDesc.vtbl = g_collisionRayVtbl;
  g_collisionRayDesc.type = 1;
  CollisionShapeBlockInit(&g_collisionRayBlock);
  CxxRegisterGlobalObject(&g_collisionQueryBlockRecords[0]);
  CollisionShapeBlockInit(&g_collisionRayBlock2);
  CxxRegisterGlobalObject(&g_collisionQueryBlockRecords[1]);

  g_collisionSegmentDesc.info = (void *)g_collisionSegmentVtbl;
  g_collisionSegmentDesc.type = 2;
  CollisionShapeBlockInit(&g_collisionSegmentBlock);
  CxxRegisterGlobalObject(&g_collisionQueryBlockRecords[2]);
  CollisionShapeBlockInit(&g_collisionSegmentBlock2);
  CxxRegisterGlobalObject(&g_collisionQueryBlockRecords[3]);

  g_btlBakuganSphereQuery.vtbl = g_collisionSphereVtbl;
  g_btlBakuganSphereQuery.type = 3;
  CollisionShapeBlockInit(&g_collisionSphereBlock);
  CxxRegisterGlobalObject(&g_collisionQueryBlockRecords[4]);
  CollisionShapeBlockInit(&g_collisionSphereBlock2);
  CxxRegisterGlobalObject(&g_collisionQueryBlockRecords[5]);

  g_collisionSweptSphereDesc.vtbl = g_collisionCapsuleVtbl;
  g_collisionSweptSphereDesc.subVtbl = g_collisionSegmentVtbl;
  g_collisionSweptSphereDesc.subType = 2;
  g_collisionSweptSphereDesc.type = 4;
  CollisionShapeBlockInit(g_collisionSweptSphereDesc.shapeBlock);
  CxxRegisterGlobalObject(&g_collisionQueryBlockRecords[6]);
  CollisionShapeBlockInit(&g_collisionSweptSphereBlock2);
  CxxRegisterGlobalObject(&g_collisionQueryBlockRecords[7]);

  g_collisionBoxDesc.vtbl = g_collisionBoxVtbl;
  g_collisionBoxDesc.invValid = 0;
  g_collisionBoxDesc.type = 6;
  CollisionShapeBlockInit(&g_collisionBoxBlock);
  CxxRegisterGlobalObject(&g_collisionQueryBlockRecords[8]);
  CollisionShapeBlockInit(&g_collisionBoxBlock2);
  CxxRegisterGlobalObject(&g_collisionQueryBlockRecords[9]);
}
