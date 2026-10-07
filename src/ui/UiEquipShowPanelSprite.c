// bdc 0x08960d68 UiEquipShowPanelSprite
#include "bdc.h"

/* Shows sprite `idx` of the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`) at once
   (no tween): visible on layer 0x10 at its saved z (`+0x3b38[idx]`), colour-add cleared, alpha 1.
    */

void UiEquipShowPanelSprite(UiEquip *self, u16 idx)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  sprites[idx]->flags |= 1;
  sprites[idx]->layerMask = 0x10;
  sprites[idx]->posZ = self->spriteZ[idx];
  sprites[idx]->addColor[0] = 0.0f;
  sprites[idx]->addColor[1] = 0.0f;
  sprites[idx]->addColor[2] = 0.0f;
  sprites[idx]->addColor[3] = 1.0f;
  sprites[idx]->alpha = 1.0f;
}
