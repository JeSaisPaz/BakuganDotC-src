// bdc 0x089e024c GfxModelSetMaterialAnimCallbackByIndex
#include "bdc.h"

/* Stores an animation callback and its argument at `+8`/`+0xc` of the state record of material
   `index`. */

void GfxModelSetMaterialAnimCallbackByIndex(GfxModel *self, s32 index, void *callback, void *arg)
{
  GmoMaterial *mat = (GmoMaterial *)GmoModelGetMaterial(self->data, index);
  GfxMaterialState *state = (GfxMaterialState *)mat->info;

  state->animCallback = callback;
  state = (GfxMaterialState *)mat->info;
  state->animArg = arg;
}
