// bdc 0x0881a430 CollisionColliderClearHit
#include "bdc.h"

/* Clears the four hit-result fields of a collider (`+0x13c`, `+0x140`, `+0x144`, `+0x148`). */

void CollisionColliderClearHit(void *collider_)

{
  CollisionCollider *collider = collider_;

  collider->ignoreCollider = NULL;
  collider->cooldown = 0;
  collider->hitTimer = 0;
  collider->hitField144 = 0;
}
