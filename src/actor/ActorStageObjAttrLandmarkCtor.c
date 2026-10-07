// bdc 0x088a6c68 ActorStageObjAttrLandmarkCtor
#include "bdc.h"

/* Constructor of the attribute (hologram) landmark stage object, kinds 0xbb..0xcc; returns `self`.
   `ActorStageObjBaseCtor``(self, kind, pos)`, vtable `g_actorStageObjAttrLandmarkVtbl`, ground
   snap of the model (`CollisionFindGroundPoint` from the model translation with mask 0x3fbf2500,
   y = ground.y - bounds.min.y via `ActorStageObjGetBounds`), stencil ref 0x14
   (`GfxModelSetStencilRef`), ambient (0.5, 0.5, 0.5, 1), mode 0, alpha 0.7, lighting on, fade 1,
   a 0x680-byte companion unit from the low heap (`BtlTargetPointLandmarkAttrCtor` at the ground
   point; NULL if the allocation fails), `createArg = arg`, then the `HologramParam` row of
   `kindIndex = kind - 0xbb` (`ActorStageObjAttrLandmarkGetParamRow`, only when kindIndex < 18,
   signed) with `auraIndex = kindIndex / 3 + 1`, `auraRadius` (`ActorStageObjAttrLandmarkGetRadius`)
   and hp = maxHp (`ActorStageObjAttrLandmarkGetMaxHp`, both called with `radiusLevel`),
   `groundPos` (model translation through `CollisionRaycastPoint`). Effects by kindIndex: 0..2
   effect 0x4a on `g_worldEffectMgr` attached to `effectPos[0..2]` at the nodes `ef_01`, `ef_02`,
   `ef_03` (the last two only if the node exists, `GfxEffectSpawnAttached`); 7 effect 0x131 and 8
   effect 0x132 (after clearing bit 4 of `renderFlags` and `depthBias` of the material
   `fz_landmark_ventus03_01`) on `g_btlUnitEffectMgr` at the object position
   (`GfxEffectSpawnWithOwner`); every effect gets `ownerBakugan = self` and `ownerId = self id`.
   Finally `ActorStageObjAttrLandmarkClearAuraFlags`, `g_attrLandmarkScanTex` =
   `GfxFindTexture("HologramEffect")` and an occlusion `CollisionBox` (0xc0 bytes, low heap, type 6)
   from the model bounds and root matrix with its inverse computed and its min/max x and z scaled by
   scale.x * 0.9.
   `effectPos[0..2]` and `groundPos` start as zero (the VFPU bank constant C720). The inverse matrix is
   the transposed rotation with translation `-(R * t)` and w of row 3 kept from the translation. */

ActorStageObjAttrLandmark *ActorStageObjAttrLandmarkCtor(ActorStageObjAttrLandmark *self, s32 kind, float *pos, s32 arg)
{
  ScePspFVector4 ground __attribute__((aligned(16)));
  ScePspFVector4 tmp __attribute__((aligned(16)));
  ScePspFVector4 node0 __attribute__((aligned(16)));
  ScePspFVector4 node1 __attribute__((aligned(16)));
  ScePspFVector4 node2 __attribute__((aligned(16)));
  BtlBakugan *unit;
  BtlBakugan *mem;
  CollisionBox *box;
  CollisionBox *boxMem;
  const HologramParam *param;
  GfxEffect *fx;
  GfxMaterialState *mat;
  GmoModel *data;
  float *bounds;
  float groundY;
  float y;
  float hp;
  bool fromLow;
  s32 index;
  ScePspFVector4 rx;
  ScePspFVector4 ry;
  ScePspFVector4 rz;
  ScePspFVector4 t;
  float *root;
  float *m;
  float dx;
  float dy;
  float dz;
  float scale;
  s32 i;

  ActorStageObjBaseCtor(&self->base, kind, pos);
  self->base.base.base.vtable = &g_actorStageObjAttrLandmarkVtbl;
  CollisionFindGroundPoint(&ground.x, &self->base.base.data->rootMatrix[12], 0x3fbf2500);
  groundY = ground.y;
  bounds = ActorStageObjGetBounds(&self->base);
  y = groundY + -bounds[1];
  ground.y = y;
  self->base.base.pos[1] = y;
  self->base.base.data->rootMatrix[13] = y;
  for (i = 0; i < 3; i++) {
    self->effectPos[i][0] = 0.0f;
    self->effectPos[i][1] = 0.0f;
    self->effectPos[i][2] = 0.0f;
    self->effectPos[i][3] = 0.0f;
  }
  self->groundPos[0] = 0.0f;
  self->groundPos[1] = 0.0f;
  self->groundPos[2] = 0.0f;
  self->groundPos[3] = 0.0f;
  GfxModelSetStencilRef(&self->base.base, 0x14);
  self->base.base.ambient[0] = 0.5f;
  self->base.base.ambient[1] = 0.5f;
  self->base.base.ambient[2] = 0.5f;
  self->base.base.ambient[3] = 1.0f;
  self->mode = 0;
  self->alpha = 0.7f;
  self->unk390 = 0;
  self->unk394 = 0;
  self->auraIndex = 0;
  self->base.base.lighting = 1;
  self->base.fade = 1.0f;
  self->unit = NULL;

  unit = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x680, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    tmp = ground;
    BtlTargetPointLandmarkAttrCtor(mem, &tmp.x);
    unit = mem;
  }
  self->unit = unit;
  self->deathHandled = 0;
  self->createArg = arg;
  *(u8 *)&self->param0 = 0; /* byte store in the original */
  self->auraType = 0;
  self->price = 0;
  self->hpLevel = 0;
  self->radiusLevel = 0;
  self->param14 = 0.0f;
  self->scroll = 0.0f;
  self->phase = 0.0f;
  index = kind - 0xbb;
  self->kindIndex = index;
  if (index < 0x12) {
    param = &g_hologramParams[ActorStageObjAttrLandmarkGetParamRow(self, self->kindIndex)];
    self->param0 = param->unk0;
    self->auraType = param->auraType;
    self->price = param->price;
    self->hpLevel = param->hpLevel;
    self->radiusLevel = param->radiusLevel;
    self->param14 = param->unk14;
    self->auraIndex = self->kindIndex / 3 + 1;
  }
  self->auraRadius = ActorStageObjAttrLandmarkGetRadius(self, self->radiusLevel);
  hp = ActorStageObjAttrLandmarkGetMaxHp(self, self->radiusLevel);
  self->base.maxHp = (s32)hp;
  self->base.hp = (s32)hp;
  self->param14Init = self->param14;
  root = &self->base.base.data->rootMatrix[12];
  self->groundPos[0] = root[0];
  self->groundPos[1] = root[1];
  self->groundPos[2] = root[2];
  self->groundPos[3] = root[3];
  CollisionRaycastPoint(self->groundPos, self->groundPos);
  self->companion = NULL;

  switch (self->kindIndex) {
  case 0:
  case 1:
  case 2:
    GfxModelGetNodeWorldPos(&self->base.base, &node0, "ef_01");
    self->effectPos[0][0] = node0.x;
    self->effectPos[0][1] = node0.y;
    self->effectPos[0][2] = node0.z;
    self->effectPos[0][3] = node0.w;
    fx = (GfxEffect *)GfxEffectSpawnAttached(g_worldEffectMgr, 0x4a, self->effectPos[0]);
    fx->ownerBakugan = self;
    if (self != NULL) {
      fx->ownerId = self->base.base.base.id;
    }
    if (GfxModelFindNodeRecord(&self->base.base, "ef_02") != NULL) {
      GfxModelGetNodeWorldPos(&self->base.base, &node1, "ef_02");
      self->effectPos[1][0] = node1.x;
      self->effectPos[1][1] = node1.y;
      self->effectPos[1][2] = node1.z;
      self->effectPos[1][3] = node1.w;
      fx = (GfxEffect *)GfxEffectSpawnAttached(g_worldEffectMgr, 0x4a, self->effectPos[1]);
      fx->ownerBakugan = self;
      if (self != NULL) {
        fx->ownerId = self->base.base.base.id;
      }
    }
    if (GfxModelFindNodeRecord(&self->base.base, "ef_03") != NULL) {
      GfxModelGetNodeWorldPos(&self->base.base, &node2, "ef_03");
      self->effectPos[2][0] = node2.x;
      self->effectPos[2][1] = node2.y;
      self->effectPos[2][2] = node2.z;
      self->effectPos[2][3] = node2.w;
      fx = (GfxEffect *)GfxEffectSpawnAttached(g_worldEffectMgr, 0x4a, self->effectPos[2]);
      fx->ownerBakugan = self;
      if (self != NULL) {
        fx->ownerId = self->base.base.base.id;
      }
    }
    break;
  case 7:
    self->effectPos[0][0] = self->base.base.pos[0];
    self->effectPos[0][1] = self->base.base.pos[1];
    self->effectPos[0][2] = self->base.base.pos[2];
    self->effectPos[0][3] = self->base.base.pos[3];
    fx = GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 0x131, self->effectPos[0], self);
    fx->ownerBakugan = self;
    if (self != NULL) {
      fx->ownerId = self->base.base.base.id;
    }
    break;
  case 8:
    mat = GfxModelFindMaterialStateBySubstr(&self->base.base, "fz_landmark_ventus03_01");
    if (mat != NULL) {
      mat->renderFlags = mat->renderFlags & 0xef;
      mat->depthBias = 0;
    }
    self->effectPos[0][0] = self->base.base.pos[0];
    self->effectPos[0][1] = self->base.base.pos[1];
    self->effectPos[0][2] = self->base.base.pos[2];
    self->effectPos[0][3] = self->base.base.pos[3];
    fx = GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 0x132, self->effectPos[0], self);
    fx->ownerBakugan = self;
    if (self != NULL) {
      fx->ownerId = self->base.base.base.id;
    }
    break;
  default:
    break;
  }

  ActorStageObjAttrLandmarkClearAuraFlags();
  g_attrLandmarkScanTex = GfxFindTexture("HologramEffect");

  box = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  boxMem = MemAlloc(0xc0, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (boxMem != NULL) {
    boxMem->vtbl = g_collisionBoxVtbl;
    boxMem->invValid = 0;
    boxMem->type = 6;
    box = boxMem;
  }
  self->collisionBox = box;
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
  m = data->rootMatrix;
  box->transform.x.x = m[0];
  box->transform.x.y = m[1];
  box->transform.x.z = m[2];
  box->transform.x.w = m[3];
  box->transform.y.x = m[4];
  box->transform.y.y = m[5];
  box->transform.y.z = m[6];
  box->transform.y.w = m[7];
  box->transform.z.x = m[8];
  box->transform.z.y = m[9];
  box->transform.z.z = m[10];
  box->transform.z.w = m[11];
  box->transform.w.x = m[12];
  box->transform.w.y = m[13];
  box->transform.w.z = m[14];
  box->transform.w.w = m[15];

  box = self->collisionBox;
  if (box->invValid == 0) {
    /* invTransform = transpose of the rotation, translation -(R * t); w of row 3 kept from t */
    rx = box->transform.x;
    ry = box->transform.y;
    rz = box->transform.z;
    t = box->transform.w;
    dx = rx.x * t.x + rx.y * t.y + rx.z * t.z;
    dy = ry.x * t.x + ry.y * t.y + ry.z * t.z;
    dz = rz.x * t.x + rz.y * t.y + rz.z * t.z;
    box->invTransform.x.x = rx.x;
    box->invTransform.x.y = ry.x;
    box->invTransform.x.z = rz.x;
    box->invTransform.x.w = 0.0f;
    box->invTransform.y.x = rx.y;
    box->invTransform.y.y = ry.y;
    box->invTransform.y.z = rz.y;
    box->invTransform.y.w = 0.0f;
    box->invTransform.z.x = rx.z;
    box->invTransform.z.y = ry.z;
    box->invTransform.z.z = rz.z;
    box->invTransform.z.w = 0.0f;
    box->invTransform.w.x = -dx;
    box->invTransform.w.y = -dy;
    box->invTransform.w.z = -dz;
    box->invTransform.w.w = t.w;
    box->invValid = 1;
  }

  scale = self->base.base.scale[0] * 0.9f;
  box = self->collisionBox;
  box->aabbMin.x = box->aabbMin.x * scale;
  box->aabbMin.z = box->aabbMin.z * scale;
  box = self->collisionBox;
  box->aabbMax.x = box->aabbMax.x * scale;
  box->aabbMax.z = box->aabbMax.z * scale;
  return self;
}
