// bdc 0x088de2fc ActorSyncColliders
#include "bdc.h"

/* Copies the model's world position (root matrix row 3 of `base.data`) to the actor matrix row
   `mtx[12..15]` and into the sphere shape of the body collider (`shapeDesc` center, raised by the
   sphere radius), refreshes the shape (virtual slot 9) and marks the second collider dirty
   (`attachDirty = 1`). */

void ActorSyncColliders(Actor *self)
{
  CollisionSphereQuery *sphere;
  const VtblEntry *update;
  const float *pos;

  pos = &self->base.data->rootMatrix[12];
  self->mtx[12] = pos[0];
  self->mtx[13] = pos[1];
  self->mtx[14] = pos[2];
  self->mtx[15] = pos[3];
  sphere = (CollisionSphereQuery *)((CollisionCollider *)self->bodyCollider)->shapeDesc;
  pos = &self->base.data->rootMatrix[12];
  sphere->center.x = pos[0];
  sphere->center.y = pos[1];
  sphere->center.z = pos[2];
  sphere->center.w = pos[3];
  sphere->center.y = sphere->center.y + sphere->radius;
  update = &sphere->vtbl[9];
  ((void (*)(void *))update->fn)((u8 *)sphere + update->delta);
  ((CollisionCollider *)self->collider2)->attachDirty = 1;
}
