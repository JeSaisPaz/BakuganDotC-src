// bdc 0x0893577c UiGauntletSetupResetSprite
#include "bdc.h"

/* Resets sprite `index` of `UiGauntletSetup` to its layout state: visible
   (flag 1), layer mask 2, depth from the saved table `spriteZ[index]`, add colour (0, 0, 0, 1) and alpha
   1. */

void UiGauntletSetupResetSprite(UiGauntletSetup *self, u16 index)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  sprites[index]->flags |= 1;
  sprites[index]->layerMask = 2;
  sprites[index]->posZ = self->spriteZ[index];
  sprites[index]->addColor[0] = 0.0f;
  sprites[index]->addColor[1] = 0.0f;
  sprites[index]->addColor[2] = 0.0f;
  sprites[index]->addColor[3] = 1.0f;
  sprites[index]->alpha = 1.0f;
}
