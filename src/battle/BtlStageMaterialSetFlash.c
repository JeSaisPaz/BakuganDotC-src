// bdc 0x0889ccf4 BtlStageMaterialSetFlash
#include "bdc.h"

/* Material visitor: installs the flash callback `BtlStageMaterialFlashCallback` with `param`
   (`GfxMaterialSetAnimCallback`) on materials whose render-flag bits `renderFlags & 0xc` are 0
   or 8. Used by `BtlStageModelSetFlash`. */

void BtlStageMaterialSetFlash(void *material, void *param)
{
    GfxMaterialState *state = material;
    u8 mode = state->renderFlags & 0xc;

    if (mode == 0 || mode == 8) {
        GfxMaterialSetAnimCallback(material, BtlStageMaterialFlashCallback, param);
    }
}
