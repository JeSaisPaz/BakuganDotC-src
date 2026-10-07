// bdc 0x0881a9bc CollisionDeleteAllColliders
#include "bdc.h"

/* Deletes all world colliders: both collider lists `0x08b00260` and `0x08b00270`
   (`CollisionDeleteColliderList`). Called when a battle or field ends (`BtlMainTaskDtor`,
   `BtlMainTeardown`, `GameFieldDtor`). */

void CollisionDeleteAllColliders(void)

{
  CollisionDeleteColliderList(g_collisionRaycastColliders.head);
  CollisionDeleteColliderList(g_collisionHitColliders.head);
  return;
}

