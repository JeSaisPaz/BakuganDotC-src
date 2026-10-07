// bdc 0x0896c6d0 UiCardEquipDimDisabledTabs
#include "bdc.h"

/* Darkens (tint 0.5) the tab label sprites of `UiCardEquip` whose bit is clear in
   the enabled-tab mask `+0x2a58`. */

void UiCardEquipDimDisabledTabs(UiCardEquip *self)

{
  int i;
  GfxSprite *sprite;

  for (i = 0; i < self->tabCount; i++) {
    if ((self->tabMask & (1 << i)) == 0) {
      sprite = ((GfxSprite **)self->base.data)[self->groups[5][0] + i];
      sprite->tint[0] = 0.5f;
      sprite->tint[1] = 0.5f;
      sprite->tint[2] = 0.5f;
      sprite->alpha = 1.0f;
    }
  }
}
