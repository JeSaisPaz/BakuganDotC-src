// bdc 0x088def88 ActorSetColliderLayer
#include "bdc.h"

/* Sets the collision layer of the two colliders (`CollisionCollider`) an actor owns at
   `bodyCollider` and `collider2` (each skipped if NULL). `CollisionRaycast` tests `1 << layer`
   against its layer mask. */

void ActorSetColliderLayer(Actor *self, s32 layer)

{
  if (self->bodyCollider != NULL) {
    ((CollisionCollider *)self->bodyCollider)->layer = layer;
  }
  if (self->collider2 != NULL) {
    ((CollisionCollider *)self->collider2)->layer = layer;
  }
}
