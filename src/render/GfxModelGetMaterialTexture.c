// bdc 0x089e01ac GfxModelGetMaterialTexture
#include "bdc.h"

/* Returns the texture bound to material `index` (via the material's layer texture slot `data+0x10 +
   (layer+0x24 & 0x3ff) * 0x10`, `+4` then `+8`); `GfxModelApplyTextureVariant` reads its name at
   `+0x18`. */

void *GfxModelGetMaterialTexture(GfxModel *self, s32 index)
{
  GmoModel *model = self->data;
  GmoMaterial *mat = (GmoMaterial *)GmoModelGetMaterial(model, index);
  GmoLayer *layers = (GmoLayer *)model->textures;

  return layers[mat->attrs->layerRef & 0x3ff].texture->palette;
}
