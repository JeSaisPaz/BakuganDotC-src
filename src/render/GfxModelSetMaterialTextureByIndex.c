// bdc 0x089e01f4 GfxModelSetMaterialTextureByIndex
#include "bdc.h"

/* Binds `texture` to material `index`: stores `texture + 0x60` (its `GmoTexture` `gmo`) in the
   material's texture slot. */

void GfxModelSetMaterialTextureByIndex(GfxModel *self, s32 index, void *texture)

{
  GmoMaterial *material;
  GmoLayer *layers;

  material = (GmoMaterial *)GmoModelGetMaterial(self->data, index);
  layers = (GmoLayer *)self->data->textures;
  layers[material->attrs->layerRef & 0x3ff].texture = &((GfxTexture *)texture)->gmo;
}
