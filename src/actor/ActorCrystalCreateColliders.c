// bdc 0x08855094 ActorCrystalCreateColliders
#include "bdc.h"

/* Creates the two collision bodies of a crystal object (`ActorCrystalCtor`): a 400-byte collider
   of kind 1 `collider0` on the capsule `bodyShape` (radius `180 * clamp(scale, 0.8, 1.0)`, axis
   `(0, scale * 730 * 1.2, 0)`) and a collider of kind 2 `collider1` on the capsule `pushShape`
   (radius `260 * scale`, axis `(0, scale * 730, 0)`), both owned by the crystal and attached to
   its `anchorMatrix`; `collider1` also gets flag bit 0, `hitTimer = 0` and a zero quad at
   `shapeDesc + 0x10`. Both are allocated from the low end of the heap under `MemLock`. Clears
   `collider2`. */

/* View of the shape descriptor of collider1: only the quad at +0x10 is written here. */
typedef struct ActorCrystalShapeDescView {
  u8 _unk00[0x10];
  float vec10[4]; /* +0x10 zeroed here */
} ActorCrystalShapeDescView;

void ActorCrystalCreateColliders(ActorCrystal *self)

{
  bool fromLow;
  CollisionCollider *mem;
  CollisionCollider *collider;
  float scale;
  float k;
  float r;
  float axisLen;
  ActorCrystalShapeDescView *desc;

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
  scale = self->base.base.scale[0];
  self->base.collider0 = collider;
  k = 0.8f;
  if (!(scale < 0.8f)) {
    k = 1.0f;
    if (scale <= 1.0f) {
      k = scale;
    }
  }
  self->base.bodyShape.start[0] = 0.0f;
  self->base.bodyShape.start[1] = 0.0f;
  self->base.bodyShape.start[2] = 0.0f;
  self->base.bodyShape.axis[0] = 0.0f;
  self->base.bodyShape.axis[1] = self->base.base.scale[0] * 730.0f * 1.2f;
  self->base.bodyShape.axis[2] = 0.0f;
  self->base.bodyShape.axisLen = 0.0f;
  r = k * 180.0f;
  self->base.bodyShape.radius = r;
  self->base.bodyShape.radiusSq = r * r;
  axisLen = __builtin_sqrtf(self->base.bodyShape.axis[0] * self->base.bodyShape.axis[0] +
                            self->base.bodyShape.axis[1] * self->base.bodyShape.axis[1] +
                            self->base.bodyShape.axis[2] * self->base.bodyShape.axis[2]);
  self->base.bodyShape.axisLen = axisLen;
  CollisionColliderInit(&self->base.collider0->node, (const u32 *)&self->base.bodyShape, 0, self, 0);
  self->base.collider0->byte104 = 0;
  collider = self->base.collider0;
  collider->attachMatrix = self->base.anchorMatrix;
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
  self->base.collider1 = collider;
  self->pushShape.start[0] = 0.0f;
  self->pushShape.start[1] = 0.0f;
  self->pushShape.start[2] = 0.0f;
  scale = self->base.base.scale[0];
  self->pushShape.axis[0] = 0.0f;
  self->pushShape.axis[1] = scale * 730.0f;
  self->pushShape.axis[2] = 0.0f;
  self->pushShape.axisLen = 0.0f;
  r = scale * 260.0f;
  self->pushShape.radius = r;
  self->pushShape.radiusSq = r * r;
  axisLen = __builtin_sqrtf(self->pushShape.axis[0] * self->pushShape.axis[0] +
                            self->pushShape.axis[1] * self->pushShape.axis[1] +
                            self->pushShape.axis[2] * self->pushShape.axis[2]);
  self->pushShape.axisLen = axisLen;
  CollisionColliderInit(&collider->node, (const u32 *)&self->pushShape, 0, self, 0);
  self->base.collider1->byte104 = 0;
  /* bank constant C720 = (0, 0, 0, 0) */
  desc = (ActorCrystalShapeDescView *)self->base.collider1->shapeDesc;
  desc->vec10[0] = 0.0f;
  desc->vec10[1] = 0.0f;
  desc->vec10[2] = 0.0f;
  desc->vec10[3] = 0.0f;
  collider = self->base.collider1;
  collider->attachMatrix = self->base.anchorMatrix;
  collider->attachDirty = 1;
  collider = self->base.collider1;
  collider->flags = collider->flags | 1;
  collider->hitTimer = 0;
  self->collider2 = NULL;
}
