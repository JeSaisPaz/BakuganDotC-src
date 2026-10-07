// bdc 0x089a6b44 UiMainMenuHookLineMaterial
#include "bdc.h"

/* Registers a material callback on the menu model for the `psp_line__BA` material (`GfxModelSetMaterialAnimCallback`
   with callback `0x089a3890` and the scroll state `+0x9a0`), which scrolls its UVs
   (`UiMainMenuScrollLine`). Same pattern as `UiWorldMapHookLineMaterial`. */

void UiMainMenuHookLineMaterial(UiMainMenu *self)

{
  GfxModel *model;
  
  model = self->models[0];
  self->lineScroll[0] = 0.0f;
  self->lineScroll[1] = 0.0f;
  self->lineScroll[2] = 0.0f;
  self->lineScroll[3] = 0.0f;
  GfxModelSetMaterialAnimCallback
            (model,"psp_line__BA",UiMainMenuTexOffsetVCallback,self->lineScroll);
  return;
}

