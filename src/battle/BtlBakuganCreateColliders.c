// bdc 0x088636bc BtlBakuganCreateColliders
#include "bdc.h"

/* Allocates and initialises the two 0x190-byte colliders of a battle unit, both from the low end
   of the heap (`MemSetAllocFromLow`): a kind-1 body collider `collider0` on the capsule
   `bodyShape` (start 0, axis `(0, offsetY, 0)`, `radius`) attached to `anchorMatrix`, and a
   kind-2 collider `collider1` on the sphere `pushShape`, whose descriptor centre is copied from
   the model root matrix translation row and raised by its radius. `pushShape.center` starts as
   (0, 0, 0, radius * radius). */

void BtlBakuganCreateColliders(BtlBakugan *self, float offsetY, float radius)

{
  bool fromLow;
  CollisionCollider *mem;
  CollisionCollider *collider;
  CollisionSphereQuery *sphere;
  const VtblEntry *entry;
  float axisLen;
  const float *root;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(400, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  collider = NULL;
  if (mem != NULL) {
    CollisionColliderCtor(&mem->node, 1);
    collider = mem;
  }
  self->collider0 = collider;
  self->bodyShape.start[0] = 0.0f;
  self->bodyShape.start[1] = 0.0f;
  self->bodyShape.start[2] = 0.0f;
  self->bodyShape.axis[0] = 0.0f;
  self->bodyShape.axis[1] = offsetY;
  self->bodyShape.axis[2] = 0.0f;
  self->bodyShape.axisLen = 0.0f;
  self->bodyShape.radius = radius;
  self->bodyShape.radiusSq = radius * radius;
  axisLen = __builtin_sqrtf(self->bodyShape.axis[0] * self->bodyShape.axis[0] +
                            self->bodyShape.axis[1] * self->bodyShape.axis[1] +
                            self->bodyShape.axis[2] * self->bodyShape.axis[2]);
  self->bodyShape.axisLen = axisLen;
  CollisionColliderInit(&collider->node, (const u32 *)&self->bodyShape, 0, self, 0);
  self->collider0->byte104 = 0;
  collider = self->collider0;
  collider->attachMatrix = self->anchorMatrix;
  collider->attachDirty = 1;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(400, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  collider = NULL;
  if (mem != NULL) {
    CollisionColliderCtor(&mem->node, 2);
    collider = mem;
  }
  self->collider1 = collider;
  /* sv.q of bank C720 (0, 0, 0, 0) */
  self->pushShape.center.x = 0.0f;
  self->pushShape.center.y = 0.0f;
  self->pushShape.center.z = 0.0f;
  self->pushShape.center.w = 0.0f;
  self->pushShape.radius = radius;
  self->pushShape.center.w = radius * radius;
  CollisionColliderInit(&self->collider1->node, (const u32 *)&self->pushShape, 0, self, 0);
  self->collider1->byte104 = 0;
  sphere = (CollisionSphereQuery *)self->collider1->shapeDesc;
  root = self->base.data->rootMatrix;
  sphere->center.x = root[12];
  sphere->center.y = root[13];
  sphere->center.z = root[14];
  sphere->center.w = root[15];
  sphere->center.y = sphere->center.y + sphere->radius;
  entry = &sphere->vtbl[9];
  ((void (*)(void *))entry->fn)((char *)sphere + entry->delta);
  collider = self->collider1;
  collider->flags = collider->flags | 1;
  collider->hitTimer = 0;
}
