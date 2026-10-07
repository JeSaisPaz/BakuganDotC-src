// bdc 0x0892c940 UiBakuganSelectMeasureNamePanel
#include "bdc.h"

/* Records the offsets of the name label sprites 0 and 7 from sprite 6 of the Bakugan select screen
   (`UiBakuganSelectCtor`, task 371; cursor `+0x74`, current entry `+0x75`, owned list `+0x1ba4`
   with 0xc-byte entries) into `namePanelOffset` (`+0x1cc0..+0x1ccc`; Y has 2.0 subtracted). */

void UiBakuganSelectMeasureNamePanel(UiBakuganSelect *self)
{
  GfxSprite **sprites = (GfxSprite **)(self->base).data;

  self->namePanelOffset[0] = sprites[6]->posX - sprites[0]->posX;
  self->namePanelOffset[1] = (sprites[6]->posY - sprites[0]->posY) - 2.0f;
  self->namePanelOffset[2] = sprites[6]->posX - sprites[7]->posX;
  self->namePanelOffset[3] = (sprites[6]->posY - sprites[7]->posY) - 2.0f;
}
