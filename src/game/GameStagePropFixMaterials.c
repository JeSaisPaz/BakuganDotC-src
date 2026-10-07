// bdc 0x088d3b38 GameStagePropFixMaterials
#include "bdc.h"

/* Runs `GameStagePropMaterialCallback` over every material of `model` (`GfxModelForEachMaterial`). */

void GameStagePropFixMaterials(void *model)

{
  GfxModelForEachMaterial(model,GameStagePropMaterialCallback,(void *)0x0);
  return;
}

