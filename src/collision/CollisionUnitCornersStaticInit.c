// bdc 0x089e7c08 CollisionUnitCornersStaticInit
#include "bdc.h"

/* Static constructor of the unit-cube corner table `0x08b02560`: eight `ScePspFVector4` corners
   (±1, ±1, ±1, w = 1) in the order (-,-,-) (+,-,-) (+,+,-) (-,+,-) (-,-,+) (+,-,+) (+,+,+)
   (-,+,+), used by `CollisionPhysBoxInit` to place the physics box corners. */

void CollisionUnitCornersStaticInit(void)

{
  g_collisionUnitCorners[0].x = -1.0f;
  g_collisionUnitCorners[0].y = -1.0f;
  g_collisionUnitCorners[0].z = -1.0f;
  g_collisionUnitCorners[0].w = 1.0f;
  g_collisionUnitCorners[1].x = 1.0f;
  g_collisionUnitCorners[1].y = -1.0f;
  g_collisionUnitCorners[1].z = -1.0f;
  g_collisionUnitCorners[1].w = 1.0f;
  g_collisionUnitCorners[2].x = 1.0f;
  g_collisionUnitCorners[2].y = 1.0f;
  g_collisionUnitCorners[2].z = -1.0f;
  g_collisionUnitCorners[2].w = 1.0f;
  g_collisionUnitCorners[3].x = -1.0f;
  g_collisionUnitCorners[3].y = 1.0f;
  g_collisionUnitCorners[3].z = -1.0f;
  g_collisionUnitCorners[3].w = 1.0f;
  g_collisionUnitCorners[4].x = -1.0f;
  g_collisionUnitCorners[4].y = -1.0f;
  g_collisionUnitCorners[4].z = 1.0f;
  g_collisionUnitCorners[4].w = 1.0f;
  g_collisionUnitCorners[5].x = 1.0f;
  g_collisionUnitCorners[5].y = -1.0f;
  g_collisionUnitCorners[5].z = 1.0f;
  g_collisionUnitCorners[5].w = 1.0f;
  g_collisionUnitCorners[6].x = 1.0f;
  g_collisionUnitCorners[6].y = 1.0f;
  g_collisionUnitCorners[6].z = 1.0f;
  g_collisionUnitCorners[6].w = 1.0f;
  g_collisionUnitCorners[7].x = -1.0f;
  g_collisionUnitCorners[7].y = 1.0f;
  g_collisionUnitCorners[7].z = 1.0f;
  g_collisionUnitCorners[7].w = 1.0f;
}
