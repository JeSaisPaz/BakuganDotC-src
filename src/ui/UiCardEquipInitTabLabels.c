// bdc 0x0896bc84 UiCardEquipInitTabLabels
#include "bdc.h"

/* Shows the tab label sprites (group 5) of `UiCardEquip` at full alpha and
   selects the cells of the per-Bakugan labels from `tabIds[1..3]`: with fewer than 3 Bakugan only
   label 3 (tabIds[1]); otherwise labels 1, 4 and 5 (tabIds[1], [2], [3]). */

void UiCardEquipInitTabLabels(UiCardEquip *self)
{
  int i;
  int rel;

  for (i = self->groups[5][0]; i < self->groups[5][0] + (s8)self->groups[5][1]; i++) {
    UiCardEquipShowSprite(self, ((GfxSprite **)self->base.data)[i]);
    ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
    rel = i - self->groups[5][0];
    if (self->bakuganCount < 3) {
      if (rel == 3) {
        GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], 0.0f, (float)self->tabIds[1]);
      }
    } else {
      switch (rel) {
      case 1:
        GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], 0.0f, (float)self->tabIds[1]);
        break;
      case 4:
        GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], 0.0f, (float)self->tabIds[2]);
        break;
      case 5:
        GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], 0.0f, (float)self->tabIds[3]);
        break;
      }
    }
  }
}
