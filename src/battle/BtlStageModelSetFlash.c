// bdc 0x0889cd44 BtlStageModelSetFlash
#include "bdc.h"

/* Turns the colour-flash material callback of an arena model on (BtlStageMaterialSetFlash with
   `param`) or off (BtlStageMaterialClearFlash, NULL argument) for every material
   (GfxModelForEachMaterial). */

void BtlStageModelSetFlash(void *model, void *param, u8 enable)
{
    if (enable != 0) {
        GfxModelForEachMaterial(model, BtlStageMaterialSetFlash, param);
        return;
    }
    GfxModelForEachMaterial(model, BtlStageMaterialClearFlash, NULL);
}
