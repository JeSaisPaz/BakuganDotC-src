// bdc 0x08914b88 UiUpgradeRefreshSlotColors
#include "bdc.h"

/* Recolours the upgrade slots 1..5 of the selected Bakugan (`bakugan`) on the upgrade screen
   (`maybe_UiScreen490Ctor`, task 490). A slot is shown white (tint 1.0) when it needs no
   prerequisite (`g_upgradePrereqFlags``[``g_upgradeClassTable``[bakugan] * 6 + slot]` == 0) or
   when its predecessor is owned in the save profile (`upgradeOwned[bakugan][slot - 1]`,
   `SaveGetProfile`), and dimmed (tint 0.3) otherwise; alpha is always 1.0. Each slot has nine
   sprites in the screen's sprite table (`base.data`): indices 44, 54, 60, 66 + slot and the five
   sprites 72 + slot * 5 .. 76 + slot * 5. */

void UiUpgradeRefreshSlotColors(UiUpgrade *self)
{
  GfxSprite *sprite;
  int slot;
  int i;
  float c;

  for (slot = 1; slot < 6; slot++) {
    if (g_upgradePrereqFlags[g_upgradeClassTable[self->bakugan] * 6 + slot] == 0 ||
        SaveGetProfile()->data->upgradeOwned[self->bakugan][slot - 1] != 0) {
      c = 1.0f;
    } else {
      c = 0.3f;
    }
    sprite = ((GfxSprite **)self->base.data)[44 + slot];
    sprite->tint[0] = c;
    sprite->tint[1] = c;
    sprite->tint[2] = c;
    sprite->alpha = 1.0f;
    sprite = ((GfxSprite **)self->base.data)[54 + slot];
    sprite->tint[0] = c;
    sprite->tint[1] = c;
    sprite->tint[2] = c;
    sprite->alpha = 1.0f;
    sprite = ((GfxSprite **)self->base.data)[60 + slot];
    sprite->tint[0] = c;
    sprite->tint[1] = c;
    sprite->tint[2] = c;
    sprite->alpha = 1.0f;
    sprite = ((GfxSprite **)self->base.data)[66 + slot];
    sprite->tint[0] = c;
    sprite->tint[1] = c;
    sprite->tint[2] = c;
    sprite->alpha = 1.0f;
    for (i = 0; i < 5; i++) {
      sprite = ((GfxSprite **)self->base.data)[72 + slot * 5 + i];
      sprite->tint[0] = c;
      sprite->tint[1] = c;
      sprite->tint[2] = c;
      sprite->alpha = 1.0f;
    }
  }
}
