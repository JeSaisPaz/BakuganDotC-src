// bdc 0x088a19f8 ActorStageObjLandmarkCtor
#include "bdc.h"

/* Constructor of the landmark stage object kind 0xb1 (`f6_landmark01_02`) and returns `self`:
   `ActorStageObjBaseCtor``(self, 0xb1, pos)`, vtable `g_actorStageObjLandmarkVtbl`, the
   embedded helper capsule (type 4 / `g_collisionCapsuleVtbl`, segment view type 2 /
   `g_collisionSegmentVtbl`; start (1,0,1), zero axis and radius), lighting on, fade 1, a
   companion unit of 0x690 bytes from the low heap (`BtlTargetPointLandmarkCtor(700.0, 250.0, unit,
   pos, 1, 2)`, NULL if the allocation fails), ground snap of `pos` and the model (`CollisionFindGroundPoint`
   from the model translation, y = 0 - bounds.min.y * scale.y via `ActorStageObjGetBounds`), ambient
   colour (0, 0.6, 0.8, 1) (`GfxModelSetAmbientColor`), the orb node and its world position
   (`orbNode`, `orbPos`), materials (`ActorStageObjLandmarkSetupMaterials`), identity `helperMatrix`
   (VFPU `vmidt`), the aura (`ActorStageObjLandmarkSpawnAura`), base alpha 1 and an occlusion
   `CollisionBox` (0xc0 bytes, low heap) from the model bounds and root matrix with its inverse
   computed, its min/max x and z scaled by scale.x * 0.9 and max.y lowered by 100 (clamped to at
   least 30).
   `knockDir` and the first three `uvScrolls` quads are zeroed (VFPU bank C720 = 0). */

ActorStageObjLandmark *ActorStageObjLandmarkCtor(ActorStageObjLandmark *self, float *pos)

{
  ScePspFVector4 tmp __attribute__((aligned(16)));
  ScePspFVector4 ground __attribute__((aligned(16)));
  float colour[4] __attribute__((aligned(16)));
  BtlTargetPointLandmark *unit;
  BtlTargetPointLandmark *mem;
  CollisionBox *box;
  CollisionBox *boxMem;
  GmoModel *data;
  const char *orbName;
  float *bounds;
  float y;
  bool fromLow;
  s32 i;
  s32 j;
  float *scroll;
  ScePspFMatrix4 *m;
  float tx;
  float ty;
  float tz;
  float k;

  ActorStageObjBaseCtor(&self->base, 0xb1, pos);
  self->base.base.base.vtable = g_actorStageObjLandmarkVtbl;
  self->capsule.vtbl = g_collisionCapsuleVtbl;
  ((SegmentShape *)self->capsule.segmentHead)->info = (void *)g_collisionSegmentVtbl;
  ((SegmentShape *)self->capsule.segmentHead)->type = 2;
  self->capsule.type = 4;
  self->step = 0;
  self->timer = 0;
  self->state = 0;
  self->base.base.lighting = 1;
  self->base.fade = 1.0f;

  unit = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc((sizeof(BtlTargetPointLandmark) + 0xf) & ~0xfu, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    tmp.x = pos[0];
    tmp.y = pos[1];
    tmp.z = pos[2];
    tmp.w = pos[3];
    BtlTargetPointLandmarkCtor(700.0f, 250.0f, mem, &tmp.x, 1, 2);
    unit = mem;
  }
  self->unit = unit;
  self->removed = 0;
  self->base.record = NULL;
  self->knockDir[0] = 0.0f;
  self->knockDir[1] = 0.0f;
  self->knockDir[2] = 0.0f;
  self->knockDir[3] = 0.0f;

  CollisionFindGroundPoint(&ground.x, &self->base.base.data->rootMatrix[12], 0x3fbf2500);
  pos[0] = ground.x;
  pos[1] = ground.y;
  pos[2] = ground.z;
  pos[3] = ground.w;
  pos[1] = 0.0f;
  bounds = ActorStageObjGetBounds(&self->base);
  y = 0.0f + -(bounds[1] * self->base.base.scale[1]);
  pos[1] = y;
  self->base.base.data->rootMatrix[13] = y;
  self->base.base.pos[1] = self->base.base.data->rootMatrix[13];

  colour[0] = 0.0f;
  colour[1] = 0.6f;
  colour[2] = 0.8f;
  colour[3] = 1.0f;
  GfxModelSetAmbientColor(&self->base.base, colour, NULL);
  for (i = 0; i < 3; i++) {
    scroll = (float *)&self->base.uvScrolls[i * 0x10];
    scroll[0] = 0.0f;
    scroll[1] = 0.0f;
    scroll[2] = 0.0f;
    scroll[3] = 0.0f;
  }
  self->orbAngle = 0.0f;

  orbName = "f6_landmark01_02";
  self->orbNode = (GmoNode *)GfxModelFindNode(&self->base.base, orbName);
  GfxModelGetNodeWorldPos(&self->base.base, &tmp, orbName);
  self->orbPos[0] = tmp.x;
  self->orbPos[1] = tmp.y;
  self->orbPos[2] = tmp.z;
  self->orbPos[3] = tmp.w;
  ActorStageObjLandmarkSetupMaterials(self);

  self->effect = NULL;
  self->helper = NULL;
  self->capsule.start[0] = 1.0f;
  self->capsule.start[1] = 0.0f;
  self->capsule.start[2] = 1.0f;
  self->capsule.axis[0] = 0.0f;
  self->capsule.axis[1] = 0.0f;
  self->capsule.axis[2] = 0.0f;
  self->capsule.axisLen = 0.0f;
  self->capsule.radius = 0.0f;
  self->capsule.radiusSq = 0.0f * 0.0f;
  self->capsule.axisLen = __builtin_sqrtf(self->capsule.axis[0] * self->capsule.axis[0] +
                                          self->capsule.axis[1] * self->capsule.axis[1] +
                                          self->capsule.axis[2] * self->capsule.axis[2]);
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      self->helperMatrix[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }
  ActorStageObjLandmarkSpawnAura(self);
  self->base.baseAlpha = 1.0f;

  box = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  boxMem = MemAlloc((sizeof(CollisionBox) + 0xf) & ~0xfu, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (boxMem != NULL) {
    boxMem->vtbl = g_collisionBoxVtbl;
    boxMem->invValid = 0;
    boxMem->type = 6;
    box = boxMem;
  }
  self->collisionMesh = box;
  /* UB (original binary): a failed allocation leaves `box` NULL and the stores below go to address 0x10.. */
  bounds = ActorStageObjGetBounds(&self->base);
  data = self->base.base.data;
  box->aabbMin.x = bounds[0];
  box->aabbMin.y = bounds[1];
  box->aabbMin.z = bounds[2];
  box->aabbMin.w = bounds[3];
  box->aabbMax.x = bounds[4];
  box->aabbMax.y = bounds[5];
  box->aabbMax.z = bounds[6];
  box->aabbMax.w = bounds[7];
  box->invValid = 0;
  box->transform = *(ScePspFMatrix4 *)data->rootMatrix;

  box = self->collisionMesh;
  if (box->invValid == 0) {
    /* invTransform = transpose of the rotation (w lanes 0), translation -(R^T * t), w kept from t */
    m = &box->transform;
    tx = m->x.x * m->w.x + m->x.y * m->w.y + m->x.z * m->w.z;
    ty = m->y.x * m->w.x + m->y.y * m->w.y + m->y.z * m->w.z;
    tz = m->z.x * m->w.x + m->z.y * m->w.y + m->z.z * m->w.z;
    box->invTransform.x.x = m->x.x;
    box->invTransform.x.y = m->y.x;
    box->invTransform.x.z = m->z.x;
    box->invTransform.x.w = 0.0f;
    box->invTransform.y.x = m->x.y;
    box->invTransform.y.y = m->y.y;
    box->invTransform.y.z = m->z.y;
    box->invTransform.y.w = 0.0f;
    box->invTransform.z.x = m->x.z;
    box->invTransform.z.y = m->y.z;
    box->invTransform.z.z = m->z.z;
    box->invTransform.z.w = 0.0f;
    box->invTransform.w.x = -tx;
    box->invTransform.w.y = -ty;
    box->invTransform.w.z = -tz;
    box->invTransform.w.w = m->w.w;
    box->invValid = 1;
  }

  k = self->base.base.scale[0] * 0.9f;
  box = self->collisionMesh;
  box->aabbMin.x = box->aabbMin.x * k;
  box->aabbMin.z = box->aabbMin.z * k;
  box = self->collisionMesh;
  box->aabbMax.x = box->aabbMax.x * k;
  box->aabbMax.z = box->aabbMax.z * k;
  self->collisionMesh->aabbMax.y = self->collisionMesh->aabbMax.y - 100.0f;
  if (self->collisionMesh->aabbMax.y <= 30.0f) {
    self->collisionMesh->aabbMax.y = 30.0f;
  }
  return self;
}
