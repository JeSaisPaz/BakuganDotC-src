// bdc 0x08854f98 ActorCrystalMaterialSetAdditive
#include "bdc.h"

/* Per-material callback (`GfxModelForEachMaterial`) of the crystal model, run by
   `ActorCrystalInitMaterials` over every material when crystal byte `+0xa42` is non-zero: sets
   bits 2..3 of the material state flag byte `+4` to 2 (the `__FD`/`__LD` render-code value,
   fog/lighting off), sets the blend mode bits 6..7 to 2 (additive, `0xdf0000a2` in
   `GmoDlWriteMeshRenderState`) and clears the alpha-test reference u16 `+0`. */

void ActorCrystalMaterialSetAdditive(GfxMaterialState *matState)
{
  matState->renderFlags = (matState->renderFlags & 0xf3) | 0x8;
  matState->renderFlags = (matState->renderFlags & 0x3f) | 0x80;
  matState->alphaRef = 0;
}
