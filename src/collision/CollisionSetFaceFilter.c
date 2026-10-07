// bdc 0x089e5448 CollisionSetFaceFilter
#include "bdc.h"

/* Selects the triangle filter callback `0x08ac5ce8` used by the mesh tests: 1
   `CollisionFaceFilterWall`, 2 `CollisionFaceFilterFloor`, 3 `CollisionFaceFilterCeiling`, 4
   `CollisionFaceFilterNotVertical`, 0 none; values above 4 leave it unchanged. Called once per
   query by `CollisionRaycast` and `CollisionHitQuery`. */

void CollisionSetFaceFilter(u32 mode)

{
  if (mode < 5) {
    if (mode == 1) {
      g_collisionFaceFilter = CollisionFaceFilterWall;
      return;
    }
    if (mode == 2) {
      g_collisionFaceFilter = CollisionFaceFilterFloor;
      return;
    }
    if (mode == 3) {
      g_collisionFaceFilter = CollisionFaceFilterCeiling;
      return;
    }
    if (mode != 4) {
      g_collisionFaceFilter = (void *)0x0;
      return;
    }
    g_collisionFaceFilter = CollisionFaceFilterNotVertical;
  }
  return;
}

