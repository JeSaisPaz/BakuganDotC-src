// bdc 0x088dbfd4 ActorCreateCollider
#include "bdc.h"

/* Creates the two colliders of a field actor (called by `ActorCtor`). Each 0x190-byte collider
   is allocated from the low end of the heap under the heap lock (`MemAlloc`) and built by
   `CollisionColliderCtor` (NULL if the allocation fails, which is not checked afterwards).
   - `collider2` (type 1) gets the capsule `collShape`: start 0, axis (0, 10, 0), radius 2.5,
     `axisLen` = |axis|; it is attached to `mtx` (`attachMatrix`, `attachDirty = 1`).
   - `bodyCollider` (type 2) gets the sphere `bodyShape`: centre and radiusSq zeroed (VFPU bank
     C720 = 0), radius 5.6 (`separationRadius`, radiusSq = 5.6^2). After `CollisionColliderInit`
     the installed sphere (`shapeDesc`) gets the model root position row
     (`base.data->rootMatrix[12..15]`, its w landing in `radiusSq`), is raised by its radius and
     refreshed (virtual slot 9, `CollisionSphereRecalc`); then flags bit0 is set and `hitTimer`
     cleared. */

void ActorCreateCollider(Actor *self)
{
  bool fromLow;
  void *mem;
  CollisionCollider *collider;
  CollisionSphere *sphere;
  const VtblEntry *update;
  const float *root;
  float radius;
  float x;
  float y;
  float z;
  float w;

  collider = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(CollisionCollider), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    CollisionColliderCtor((CoreNode *)mem, 1);
    collider = (CollisionCollider *)mem;
  }
  self->collider2 = collider;
  self->collShape.start[0] = 0.0f;
  self->collShape.start[1] = 0.0f;
  self->collShape.start[2] = 0.0f;
  self->collShape.axis[0] = 0.0f;
  self->collShape.axis[1] = 10.0f;
  self->collShape.axis[2] = 0.0f;
  self->collShape.axisLen = 0.0f;
  radius = 2.5f;
  self->collShape.radius = radius;
  self->collShape.radiusSq = radius * radius;
  x = self->collShape.axis[0];
  y = self->collShape.axis[1];
  z = self->collShape.axis[2];
  self->collShape.axisLen = __builtin_sqrtf(x * x + y * y + z * z);
  CollisionColliderInit(&collider->node, (const u32 *)&self->collShape, 0, self, 0);
  ((CollisionCollider *)self->collider2)->byte104 = 0;
  collider = (CollisionCollider *)self->collider2;
  collider->attachMatrix = (float (*)[4])self->mtx;
  collider->attachDirty = 1;

  collider = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(CollisionCollider), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    CollisionColliderCtor((CoreNode *)mem, 2);
    collider = (CollisionCollider *)mem;
  }
  self->bodyCollider = collider;
  self->bodyShape.center[0] = 0.0f;
  self->bodyShape.center[1] = 0.0f;
  self->bodyShape.center[2] = 0.0f;
  self->bodyShape.radiusSq = 0.0f;
  radius = 5.6f;
  self->separationRadius = radius;
  self->bodyShape.radiusSq = radius * radius;
  CollisionColliderInit(&((CollisionCollider *)self->bodyCollider)->node,
                        (const u32 *)&self->bodyShape, 0, self, 0);
  ((CollisionCollider *)self->bodyCollider)->byte104 = 0;
  sphere = (CollisionSphere *)((CollisionCollider *)self->bodyCollider)->shapeDesc;
  root = &self->base.data->rootMatrix[0xc];
  x = root[0];
  y = root[1];
  z = root[2];
  w = root[3];
  sphere->center[0] = x;
  sphere->center[1] = y;
  sphere->center[2] = z;
  sphere->radiusSq = w;
  sphere->center[1] = sphere->center[1] + sphere->radius;
  update = &sphere->vtbl[9];
  ((void (*)(void *))update->fn)((u8 *)sphere + update->delta);
  collider = (CollisionCollider *)self->bodyCollider;
  collider->flags = collider->flags | 1;
  collider->hitTimer = 0;
}
