// bdc 0x0895b854 UiEquipLoadSlotModel
#include "bdc.h"

/* Loads the 3D Bakugan model shown on player `slot`'s pedestal of the Bakugan/gear loadout screen
   (task 302, `UiEquipCtor`). Clears `float4f7c`, writes the select motion name of Bakugan `id`
   into `motionNames[slot]` (`UiBakuganGetSelectMotionName`, or `UiBakuganGetSelectTurnMotionName`
   for a mirrored slot without alt motion), allocates a 0x140-byte `GfxModelCtor` model of
   `g_btlModelNames``[id]` (low-heap allocation) into `bakuganModels[slot]`, enables and plays the
   motion (blend 0.2, looped), runs the model's virtual motion update (vtable slot 7), tags it with
   `id`, sets up its shading, scales its root matrix by `UiBakuganGetModelScale` (form 0 for up to
   2 players, else 1), rotates it (`UiBakuganRotateModel`), adds the pedestal offset to its position
   and copies that into the matrix translation (w = 1), sets motion speed 1.0 (virtual slot 6) and
   ambient alpha 0. With an alt motion it runs `UiEquipAltModelMaterialCallback` on every material
   and mirrors the matrix X row. Finally applies texture variant 0 for slot 0, or for slots 1..3 the
   number of earlier players that picked the same Bakugan; slots >= 4 get no variant. */

void UiEquipLoadSlotModel(UiEquip *self, u8 slot, u8 id)

{
  char *name;
  bool fromLow;
  GfxModel *model;
  GfxModel *alloc;
  s32 motion;
  const VtblEntry *vt;
  float *mtx;
  float scale;
  float offset[2];
  float rowScale[4];
  int r;
  int k;
  u32 i;
  u8 variant;

  name = self->motionNames[slot];
  self->float4f7c = 0.0f;
  memset(name, 0, 0x40);
  if (!UiEquipIsMirroredSlot(self, slot)) {
    UiBakuganGetSelectMotionName(id, name);
  }
  else if (UiEquipHasAltMotion(self, slot, id) != 0) {
    UiBakuganGetSelectMotionName(id, name);
  }
  else {
    UiBakuganGetSelectTurnMotionName(id, name);
  }

  model = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  alloc = MemAlloc(0x140, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (alloc != NULL) {
    GfxModelCtor(alloc, g_btlModelNames[id], 0);
    model = alloc;
  }
  self->bakuganModels[slot] = model;
  GfxModelEnableMotion(model);

  model = self->bakuganModels[slot];
  motion = GmoMotionIndexOfName(GmoMotionMgrGet(), name);
  GfxModelPlayMotion(0.2f, model, motion, 1);

  model = self->bakuganModels[slot];
  vt = &((const VtblEntry *)model->base.vtable)[7];
  ((void (*)(void *))vt->fn)((u8 *)model + vt->delta);
  self->bakuganModels[slot]->base.unk08 = id;
  BtlBakuganSetupModelShading(self->bakuganModels[slot]);

  if (self->playerCount < 3) {
    mtx = self->bakuganModels[slot]->data->rootMatrix;
    scale = UiBakuganGetModelScale(id, 0);
  }
  else {
    mtx = self->bakuganModels[slot]->data->rootMatrix;
    scale = UiBakuganGetModelScale(id, 1);
  }
  mtx[10] = scale;
  mtx[5] = scale;
  mtx[0] = scale;

  model = self->bakuganModels[slot];
  UiBakuganRotateModel(model, id, UiEquipIsMirroredSlot(self, slot));

  UiEquipGetPedestalOffset(offset, &self->base, slot);
  self->bakuganModels[slot]->pos[0] = self->bakuganModels[slot]->pos[0] + offset[0];
  self->bakuganModels[slot]->pos[1] = self->bakuganModels[slot]->pos[1] + offset[1];
  model = self->bakuganModels[slot];
  /* copy pos (4 floats) into the translation row */
  for (k = 0; k < 4; k++) {
    model->data->rootMatrix[12 + k] = model->pos[k];
  }
  self->bakuganModels[slot]->data->rootMatrix[15] = 1.0f;

  model = self->bakuganModels[slot];
  vt = &((const VtblEntry *)model->base.vtable)[6];
  ((void (*)(void *, float))vt->fn)((u8 *)model + vt->delta, 1.0f);
  self->bakuganModels[slot]->ambient[3] = 0.0f;

  if (UiEquipHasAltMotion(self, slot, id) != 0) {
    GfxModelForEachMaterial(self->bakuganModels[slot], (void *)UiEquipAltModelMaterialCallback,
                            NULL);
    mtx = self->bakuganModels[slot]->data->rootMatrix;
    rowScale[0] = -1.0f;
    rowScale[1] = 1.0f;
    rowScale[2] = 1.0f;
    rowScale[3] = 0.0f;
    /* scale matrix rows 0..2 by rowScale[0..2] (row 0 mirrored) */
    for (r = 0; r < 3; r++) {
      for (k = 0; k < 4; k++) {
        mtx[r * 4 + k] = mtx[r * 4 + k] * rowScale[r];
      }
    }
  }

  if (slot == 0) {
    GfxModelApplyTextureVariant(self->bakuganModels[slot], 0);
  }
  else if (slot < 4) {
    variant = 0;
    for (i = 0; i < slot; i++) {
      if ((s8)self->bakuganPick[i] == id) {
        variant++;
      }
    }
    GfxModelApplyTextureVariant(self->bakuganModels[slot], variant);
  }
  return;
}
