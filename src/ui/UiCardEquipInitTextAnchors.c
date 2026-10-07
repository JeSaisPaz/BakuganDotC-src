// bdc 0x0896cf38 UiCardEquipInitTextAnchors
#include "bdc.h"

/* Prepares the text anchor sprites (group 19, one per Bakugan) of `UiCardEquip`,
   hidden; their positions place the card name and help text. */

void UiCardEquipInitTextAnchors(UiCardEquip *self)
{
  GfxSprite **sprites;
  s32 i;

  for (i = self->groups[19][0]; i < self->groups[19][0] + self->groups[19][1]; i++) {
    if (i - self->groups[19][0] < self->bakuganCount) {
      sprites = (GfxSprite **)self->base.data;
      UiCardEquipShowSprite(self, sprites[i]);
      sprites[i]->flags = sprites[i]->flags & ~1u;
    }
  }
}
