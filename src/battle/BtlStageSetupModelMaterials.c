// bdc 0x0889cb90 BtlStageSetupModelMaterials
#include "bdc.h"

/* Runs the material callback `BtlStageModelMaterialCallback` on every material of an arena
   model (`GfxModelForEachMaterial`, argument NULL). Called by `BtlStageLoadMap` after
   creating each map model. */

void BtlStageSetupModelMaterials(void *model)
{
    GfxModelForEachMaterial(model, BtlStageModelMaterialCallback, NULL);
}
