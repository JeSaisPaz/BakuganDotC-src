// bdc 0x089144c4 UiUpgradeHideCursors
#include "bdc.h"

/* Hides the cursor sprites at slots 50, 53 and 102 of the sprite table in `data` of the Bakugan
   upgrade screen (`UiUpgradeCtor`, task 490) by clearing bit 0 of their `flags`. */

void UiUpgradeHideCursors(UiUpgrade *self)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  sprites[0xc8 / 4]->flags &= ~1u;
  sprites[0xd4 / 4]->flags &= ~1u;
  sprites[0x198 / 4]->flags &= ~1u;
}
