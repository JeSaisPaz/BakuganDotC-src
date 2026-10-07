// bdc 0x0889ccbc BtlStageMaterialClearFlash
#include "bdc.h"

/* Material visitor (`GfxMaterialState`): removes the animation callback
   (`GfxMaterialSetAnimCallback` with a NULL callback and argument) from materials whose blend
   mode bits `renderFlags & 0xc` are 0 or 8. Used by `BtlStageModelSetFlash` to switch the flash
   off. */
void BtlStageMaterialClearFlash(void *material)
{
    GfxMaterialState *state = (GfxMaterialState *)material;
    u32 blend = state->renderFlags & 0xc;

    if (blend == 0 || blend == 8) {
        GfxMaterialSetAnimCallback(material, NULL, NULL);
    }
}
