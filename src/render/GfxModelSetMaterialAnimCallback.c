// bdc 0x089e029c GfxModelSetMaterialAnimCallback
#include "bdc.h"

/* Finds the first material whose name contains `name` (`GfxModelFindMaterialIndexBySubstr2`) and
   stores `callback`/`arg` in its state record (`+8`/`+0xc`); returns 1 when found.
   `ActorCrystalInitMaterials` uses it for the UV-scroll callbacks of `mat_context00` etc. */

bool GfxModelSetMaterialAnimCallback(GfxModel *self, const char *name, void *callback, void *arg)

{
  s32 index;
  GmoMaterial *mat;

  index = GfxModelFindMaterialIndexBySubstr2(self, name);
  if (index != -1) {
    mat = (GmoMaterial *)GmoModelGetMaterial(self->data, index);
    ((GfxMaterialState *)mat->info)->animCallback = callback;
    ((GfxMaterialState *)mat->info)->animArg = arg;
    return true;
  }
  return false;
}
