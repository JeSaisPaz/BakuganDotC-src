// bdc 0x088238d4 GfxEffectMaterialSetBlend
#include "bdc.h"

/* `GfxModelForEachMaterial` callback used by `GfxEffectMgrDrawModels`: stores the low two bits
   of `*mode` (the effect's blend mode) in bits 6–7 of the material's flag byte `+4`. */

void GfxEffectMaterialSetBlend(void *material, u32 *mode)

{
  GfxMaterialState *mat = (GfxMaterialState *)material;

  mat->renderFlags = (mat->renderFlags & 0x3f) | (u8)((*mode & 3) << 6);
  return;
}

