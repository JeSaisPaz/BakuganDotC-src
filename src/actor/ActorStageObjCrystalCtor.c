// bdc 0x088b4048 ActorStageObjCrystalCtor
#include "bdc.h"

/* Constructor of the breakable crystal stage object (0x3a0 bytes, kind 0xa9, models `fz_crystal01`,
   `fz_crystal02`, `fz_crystal03_01`, `fz_crystal05`): `ActorStageObjBaseCtor``(obj, 0xa9, pos)`,
   vtable `g_actorStageObjCrystalVtbl`, element `variant`, record type `typeId`, 150 HP with gauge
   (`ActorStageObjEnsureHpGauge`), idle countdown 150 + `CoreRandNext``(120)`, a companion
   target-point unit (`BtlTargetPointCtor``(70, 50, …, pos, true, 5)`, NULL if the allocation
   fails), clears `knockDir` (VFPU bank zero C720), snaps `pos[1]` and the model onto the ground with
   `ActorStageObjGetBounds`, arms the crystal at once for `variant == 2`, sets material flags, the
   stage 0/13 ambient/colour, scales node `fz_crystal05` by 10 and tints the model by the element
   (`ActorCrystalGetStyleColor`, `GfxModelSetAmbientColor`, then
   `ActorStageObjCrystalSetupMaterials`). Returns `self`. */

ActorStageObjCrystal *ActorStageObjCrystalCtor(ActorStageObjCrystal *self, float *pos, s32 variant, u16 typeId)
{
  float unitPos[4];
  float tmp[4];
  float style[4];
  float colour[4];
  GfxModel *model = &self->base.base;
  GfxMaterialState *mat;
  GmoNode *node;
  float *nodeMatrix;
  void *unit;
  void *mem;
  bool fromLow;
  float *bounds;
  float y;
  float t;
  int i;

  ActorStageObjBaseCtor(&self->base, 0xa9, pos);
  model->base.vtable = g_actorStageObjCrystalVtbl;
  self->step = 0;
  self->state = 0;
  self->element = variant;
  self->typeId = typeId;
  self->fieldEnabled = 1;
  self->base.maxHp = 150;
  self->base.hp = 150;
  ActorStageObjEnsureHpGauge(&self->base);
  self->idleTimer = CoreRandNext(120) + 150;
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
    for (i = 0; i < 4; i++) {
      unitPos[i] = pos[i];
    }
    BtlTargetPointCtor(70.0f, 50.0f, (BtlBakugan *)mem, unitPos, true, 5);
    unit = mem;
  }
  self->unit = unit;
  self->destroyed = 0;
  self->base.record = NULL;
  self->reserved330 = 0;
  self->reserved324 = 0;
  /* C720 is the VFPU bank zero vector */
  for (i = 0; i < 4; i++) {
    self->knockDir[i] = 0.0f;
  }
  self->removeTimer = 0;

  /* ground snap */
  y = pos[1];
  bounds = ActorStageObjGetBounds(&self->base);
  y = y + -(bounds[1] * model->scale[1]);
  pos[1] = y;
  model->data->rootMatrix[13] = y;
  model->pos[1] = model->data->rootMatrix[13];

  self->active = 0;
  if (variant == 2) {
    self->active = 1;
  }

  mat = (GfxMaterialState *)GfxModelFindMaterialStateBySubstr(model, "fz_crystal03_01");
  if (mat != NULL) {
    mat->shadeFlags = (mat->shadeFlags & ~0xe0) | 0x20;
    mat->renderFlags = (mat->renderFlags & ~3) | 2;
  }
  GfxModelFindMaterialStateBySubstr(model, "fz_crystal01");
  mat = (GfxMaterialState *)GfxModelFindMaterialStateBySubstr(model, "fz_crystal02");
  if (mat != NULL) {
    mat->shadeFlags &= ~3;
  }

  model->lighting = 1;
  if (GameStageIs0Or13() != 0) {
    tmp[0] = 0.07f;
    tmp[1] = 0.02f;
    tmp[2] = 0.17f;
    tmp[3] = 1.0f;
    for (i = 0; i < 4; i++) {
      model->ambient[i] = tmp[i];
    }
    tmp[0] = 0.45f;
    tmp[1] = 0.45f;
    tmp[2] = 0.55f;
    tmp[3] = 1.0f;
    for (i = 0; i < 4; i++) {
      model->color[i] = tmp[i];
    }
  }

  /* node fz_crystal05: diagonal 10 and rows 1/2 swapped in both 4x4 matrices at +0x30 and +0x80
     (the +0x30 one overlays GmoNode.matrix and the following fields, see Notes) */
  node = (GmoNode *)GfxModelFindNode(model, "fz_crystal05");
  nodeMatrix = (float *)&node->matrix;
  nodeMatrix[10] = 10.0f;
  nodeMatrix[5] = 10.0f;
  nodeMatrix[0] = 10.0f;
  node->localMatrix[10] = 10.0f;
  node->localMatrix[5] = 10.0f;
  node->localMatrix[0] = 10.0f;
  for (i = 0; i < 4; i++) {
    t = nodeMatrix[4 + i];
    nodeMatrix[4 + i] = nodeMatrix[8 + i];
    nodeMatrix[8 + i] = t;
  }
  for (i = 0; i < 4; i++) {
    t = node->localMatrix[4 + i];
    node->localMatrix[4 + i] = node->localMatrix[8 + i];
    node->localMatrix[8 + i] = t;
  }

  ActorCrystalGetStyleColor(style, self->element);
  for (i = 0; i < 4; i++) {
    colour[i] = style[i];
  }
  GfxModelSetAmbientColor(model, colour, NULL);
  ActorStageObjCrystalSetupMaterials(self);
  return self;
}
