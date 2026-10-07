// bdc 0x0896bbe8 UiCardEquipInitTabButtons
#include "bdc.h"

/* Shows the tab button sprites (group 4) of `UiCardEquip` at full alpha. */

void UiCardEquipInitTabButtons(UiCardEquip *self)
{
  int i;

  for (i = self->groups[4][0]; i < self->groups[4][0] + (s8)self->groups[4][1]; i++) {
    UiCardEquipShowSprite(self, ((GfxSprite **)self->base.data)[i]);
    ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
  }
}
