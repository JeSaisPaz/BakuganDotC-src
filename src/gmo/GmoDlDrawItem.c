// bdc 0x089db5fc GmoDlDrawItem
#include "bdc.h"

/* Draws one draw item (`ctx+0x1c`): if its mask `+0x2c` intersects the model mask `+0x34` in both
   halves, resolves its material (index into `model+0xc`) into `ctx+0x20` and draws the meshes with
   `GmoDlDrawMeshes`. Returns the updated dirty mask (`dirty | previous ctx+0x40 | mesh +0x28`), or
   the result of GmoDlDrawMeshes when the item is drawn. */

u32 GmoDlDrawItem(GmoDlContext *self, GmoModel *model, u32 dirty)

{
  GmoMesh *mesh;
  u32 prevState;
  u32 state;
  u32 mask;
  u32 idx;
  void *material;

  mesh = self->mesh;
  prevState = self->lastMeshState;
  state = mesh->flags;
  self->lastMeshState = state;
  mask = model->meshMask & mesh->drawMask;
  if (((mask & 0xffff) != 0) && ((mask & 0xffff0000) != 0)) {
    idx = mesh->materialIndex;
    material = (void *)(uintptr_t)idx;
    if (((idx + 1) & 0xffff0000) == 0) {
      if ((idx & 0xffff) < (u32)model->materialCount) {
        material = (GmoMaterial *)model->materials + idx;
      } else {
        material = (void *)0;
      }
    }
    self->drawMaterial = material;
    if (material != (void *)0) {
      return GmoDlDrawMeshes(self, model, dirty | prevState | state);
    }
  }
  return dirty | prevState | state;
}
