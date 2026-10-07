// bdc 0x089e07e8 GfxModelSetMaterialVisible
#include "bdc.h"

/* Sets bit 0x20 (visible, tested by `GmoDlDrawMeshes`) of the state byte `+4` of material `index`
   to `visible & 1`. */

void GfxModelSetMaterialVisible(GfxModel *self, s32 index, bool visible)
{
  GmoMaterial *mat = (GmoMaterial *)GmoModelGetMaterial(self->data, index);
  GfxMaterialState *state = (GfxMaterialState *)mat->info;

  state->renderFlags = (state->renderFlags & 0xdf) | ((visible & 1) << 5);
}
