// bdc 0x089a6b8c UiMainMenuHookSeaMaterial
#include "bdc.h"

/* Registers the UV-scroll material callback (`0x089a38bc`, state `+0x9b0`) for the `psp_sea__DS2`
   material of the menu model (`UiMainMenuScrollSea`). Same pattern as
   `UiWorldMapHookSeaMaterial`. */

void UiMainMenuHookSeaMaterial(UiMainMenu *self)

{
  GfxModel *model;
  
  model = self->models[0];
  self->seaScroll[0] = 0.0f;
  self->seaScroll[1] = 0.0f;
  self->seaScroll[2] = 0.0f;
  self->seaScroll[3] = 0.0f;
  GfxModelSetMaterialAnimCallback
            (model,"psp_sea__DS2",UiMainMenuTexOffsetUCallback,self->seaScroll);
  return;
}

