// bdc 0x089709f4 UiOptionInitRowPanels
#include "bdc.h"

/* Shows the row panel sprites 0x1b..0x1e of `UiOption` and saves their Y positions.
    */

void UiOptionInitRowPanels(UiOption *self)

{
  int i;

  for (i = 27; i < 31; i++) {
    UiOptionShowSprite(&self->base, ((GfxSprite **)self->base.data)[i]);
    ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
    self->spriteY[i] = ((GfxSprite **)self->base.data)[i]->posY;
  }
}
