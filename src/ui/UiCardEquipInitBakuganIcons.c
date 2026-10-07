// bdc 0x0896b9d0 UiCardEquipInitBakuganIcons
#include "bdc.h"

/* Initialises the Bakugan icon sprites (group 1) of `UiCardEquip`: shows each
   one at alpha 1, hides those past the loadout's Bakugan count, and picks the cell from the
   tab-id list `tabIds[i]` (`GfxSpriteSetCell`). */

void UiCardEquipInitBakuganIcons(UiCardEquip *self)
{
  int i;
  GfxSprite *sprite;

  for (i = self->groups[1][0]; i < self->groups[1][0] + (s8)self->groups[1][1]; i++) {
    UiCardEquipShowSprite(self, ((GfxSprite **)self->base.data)[i]);
    sprite = ((GfxSprite **)self->base.data)[i];
    if (self->bakuganCount - 1 < i - self->groups[1][0]) {
      sprite->flags &= ~1u;
      sprite = ((GfxSprite **)self->base.data)[i];
    }
    sprite->alpha = 1.0f;
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], 0.0f,
                     (float)self->tabIds[i - self->groups[1][0]]);
  }
}
