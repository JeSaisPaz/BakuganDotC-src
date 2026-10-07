// bdc 0x08935b40 UiGauntletSetupLoadPlayerModel
#include "bdc.h"

/* Loads the player character model shown on the gauntlet setup screen (`UiGauntletSetup`,
   task 373): allocates a 0x140-byte `GfxModel` from the low end of the heap and constructs it
   from `"12_Edit_man.gmo"` (`GfxModelCtor`; `playerModel` stays NULL when the allocation fails
   and is still used), sets the root matrix to a Y rotation by angle 0 * S703 (identity) and scales
   its first three rows by (1, 1, 1), zeroes `pos`, sets the motion speed to 0.5 (vtable slot 6),
   turns `lighting` on, sets specular colour (0.4, 0.4, 0.4, 1) with power 8
   (`GfxModelSetSpecular`), copies `pos` into the matrix translation row (w = 1), sets
   `ambient[3]` to 1, loads `"12_editm_see_gauntlet.gmo"` and `"12_editm_see_gauntlet_push.gmo"`,
   enables motions and plays `"12_editm_see_gauntlet"` looped at frame 0.2 (`GfxModelPlayMotionByName`), then
   runs vtable slots 6 (0.5) and 7.
   Bank constant S703 = 2/pi scales the (zero) rotation angle. */

void UiGauntletSetupLoadPlayerModel(UiGauntletSetup *self)
{
  float scale[4];
  float colour[4];
  GfxModel *model;
  GfxModel *alloc;
  const VtblEntry *slot;
  bool fromLow;

  model = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  alloc = MemAlloc(sizeof(GfxModel), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (alloc != NULL) {
    GfxModelCtor(alloc, "12_Edit_man.gmo", 0);
    model = alloc;
  }
  self->playerModel = model;

  /* Y rotation by angle 0 (vrot quarter turns of 0 * 2/pi) -> rootMatrix */
  {
    float *m = model->data->rootMatrix;
    float angle = 0.0f;
    float c = __builtin_cosf(angle);
    float s = __builtin_sinf(angle);

    m[0] = c;    m[1] = 0.0f; m[2] = -s;   m[3] = 0.0f;
    m[4] = 0.0f; m[5] = 1.0f; m[6] = 0.0f; m[7] = 0.0f;
    m[8] = s;    m[9] = 0.0f; m[10] = c;   m[11] = 0.0f;
    m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
  }

  scale[2] = 1.0f;
  scale[1] = 1.0f;
  scale[0] = 1.0f;
  scale[3] = 0.0f;
  {
    float *m = ((GfxModel *)self->playerModel)->data->rootMatrix;
    int row;
    int i;

    for (row = 0; row < 3; row++) {
      for (i = 0; i < 4; i++) {
        m[row * 4 + i] = m[row * 4 + i] * scale[row];
      }
    }
  }

  ((GfxModel *)self->playerModel)->pos[0] = 0.0f;
  ((GfxModel *)self->playerModel)->pos[1] = 0.0f;
  ((GfxModel *)self->playerModel)->pos[2] = 0.0f;
  model = self->playerModel;
  slot = &((const VtblEntry *)model->base.vtable)[6];
  ((void (*)(float, void *))slot->fn)(0.5f, (u8 *)model + slot->delta);

  ((GfxModel *)self->playerModel)->lighting = 1;
  colour[0] = 0.4f;
  colour[1] = 0.4f;
  colour[2] = 0.4f;
  colour[3] = 1.0f;
  GfxModelSetSpecular(8.0f, self->playerModel, colour, NULL);

  model = self->playerModel;
  /* pos -> rootMatrix row 3 (quad copy) */
  model->data->rootMatrix[12] = model->pos[0];
  model->data->rootMatrix[13] = model->pos[1];
  model->data->rootMatrix[14] = model->pos[2];
  model->data->rootMatrix[15] = model->pos[3];
  ((GfxModel *)self->playerModel)->data->rootMatrix[15] = 1.0f;
  ((GfxModel *)self->playerModel)->ambient[3] = 1.0f;

  GmoMotionLoadFile(GmoMotionMgrGet(), "12_editm_see_gauntlet.gmo");
  GmoMotionLoadFile(GmoMotionMgrGet(), "12_editm_see_gauntlet_push.gmo");
  GfxModelEnableMotion(self->playerModel);
  GfxModelPlayMotionByName(0.2f, self->playerModel, "12_editm_see_gauntlet", true);

  model = self->playerModel;
  slot = &((const VtblEntry *)model->base.vtable)[6];
  ((void (*)(float, void *))slot->fn)(0.5f, (u8 *)model + slot->delta);
  model = self->playerModel;
  slot = &((const VtblEntry *)model->base.vtable)[7];
  ((void (*)(void *))slot->fn)((u8 *)model + slot->delta);
}
