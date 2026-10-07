// bdc 0x088a25b4 ActorStageObjActivate
#include "bdc.h"

/* Switches a landmark stage object "on": starts its looping effect (`ActorStageObjStartEffect`),
   allocates its 400-byte helper collider on first use (low-heap `MemAlloc` +
   `CollisionColliderCtor`), makes the companion unit drop every target link to it
   (`BtlBakuganReleaseLinksTo`) and marks it untargetable, fills the capsule shape at `+0x330`
   (start (0.1,0,0), axis (0,700,0) with its VFPU length, radius 230) and registers it with the
   collider (`CollisionColliderInit`, layer 9, owner = obj), copies the unit's anchor matrix to
   `helperMatrix` as the collider's attach matrix, zeroes the vector at
   `shapeDesc + 0x10` (bank constant C720) and refreshes the model matrix (`ActorStageObjUpdateTransform`).
   Counterpart: `ActorStageObjDeactivate`. */

typedef struct StageObjHelperShapeDesc {
  u8 _unk00[0x10];
  float vec10[4]; /* +0x10 receives C720 */
} StageObjHelperShapeDesc;

void ActorStageObjActivate(void *obj)

{
  ActorStageObjLandmark *self = (ActorStageObjLandmark *)obj;
  CollisionCollider *collider;
  bool fromLow;
  float radius;
  float axisLen;
  StageObjHelperShapeDesc *shapeDesc;
  int i;
  int j;

  ActorStageObjStartEffect(obj);
  if (self->helper == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    collider = (CollisionCollider *)MemAlloc(400, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (collider != NULL) {
      CollisionColliderCtor(&collider->node, 1);
    }
    self->helper = collider;
  }
  BtlBakuganReleaseLinksTo(self->unit);
  self->unit->untargetable = 1;
  self->capsule.start[0] = 0.1f;
  self->capsule.start[1] = 0.0f;
  self->capsule.start[2] = 0.0f;
  self->capsule.axis[0] = 0.0f;
  self->capsule.axis[1] = 700.0f;
  self->capsule.axis[2] = 0.0f;
  self->capsule.axisLen = 0.0f;
  radius = 230.0f;
  self->capsule.radius = radius;
  self->capsule.radiusSq = radius * radius;
  /* Length of the axis xyz (vdot.t / vsqrt.s). */
  axisLen = __builtin_sqrtf(self->capsule.axis[0] * self->capsule.axis[0] +
                            self->capsule.axis[1] * self->capsule.axis[1] +
                            self->capsule.axis[2] * self->capsule.axis[2]);
  self->capsule.axisLen = axisLen;
  CollisionColliderInit(&self->helper->node, (const u32 *)&self->capsule, 9, obj, 0);
  /* Matrix copy through C000..C030. */
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      self->helperMatrix[i][j] = self->unit->base.anchorMatrix[i][j];
    }
  }
  self->helper->attachMatrix = self->helperMatrix;
  self->helper->attachDirty = 1;
  /* sv.q of the bank constant C720 = (0, 0, 0, 0). */
  shapeDesc = (StageObjHelperShapeDesc *)self->helper->shapeDesc;
  shapeDesc->vec10[0] = 0.0f;
  shapeDesc->vec10[1] = 0.0f;
  shapeDesc->vec10[2] = 0.0f;
  shapeDesc->vec10[3] = 0.0f;
  self->helper->byte104 = 0;
  self->helper->attachDirty = 1;
  ActorStageObjUpdateTransform(&self->base);
  return;
}
