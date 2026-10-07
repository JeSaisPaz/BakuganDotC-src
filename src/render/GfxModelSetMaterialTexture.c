// bdc 0x089df80c GfxModelSetMaterialTexture
#include "bdc.h"

/* Binds a texture to the material of a GMO model instance that has the given name: finds the
   material index with `GfxModelFindMaterialIndexBySubstr(model, name)` and, if present, calls `GfxModelSetMaterialTextureByIndex(model,
   index, texture)`; returns 1 on success, 0 when the material does not exist. */

s32 GfxModelSetMaterialTexture(GfxModel *self, const char *materialName, void *texture)

{
  s32 index;
  
  index = GfxModelFindMaterialIndexBySubstr(self,materialName);
  if (index != -1) {
    GfxModelSetMaterialTextureByIndex(self,index,texture);
    return 1;
  }
  return 0;
}

