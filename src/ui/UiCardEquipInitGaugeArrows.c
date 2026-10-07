// bdc 0x0896beec UiCardEquipInitGaugeArrows
#include "bdc.h"

/* Shows the gauge arrow sprites (group 11, one per Bakugan) of `UiCardEquip`. */

void UiCardEquipInitGaugeArrows(UiCardEquip *self)
{
  GfxSprite **sprites;
  s32 i;

  for (i = self->groups[11][0]; i < self->groups[11][0] + self->groups[11][1]; i++) {
    if (i - self->groups[11][0] < self->bakuganCount) {
      sprites = (GfxSprite **)self->base.data;
      UiCardEquipShowSprite(self, sprites[i]);
      sprites[i]->alpha = 1.0f;
    }
  }
}
