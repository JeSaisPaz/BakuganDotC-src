// bdc 0x0893580c UiGauntletSetupRefreshSlotSprites
#include "bdc.h"

/* Rebuilds the card-slot sprites of `UiGauntletSetup` from the slot state
   after a change: resets (`UiGauntletSetupResetSprite`) sprites 0x0a–0x0d and 0x32–0x35 and
   hides those whose slot mark (`slotMark[slot]` bit 0) is clear; sprites 0x28–0x29 show the two
   chosen cards `chosenCard[0..1]` as small card images (`UiCardSetThumbnailTexture`,
   `"card_SS_%03d"`) or are hidden when 0xff; sprites 0x18–0x1b get button icon 2
   (`UiSetButtonIcon`) and are hidden for unmarked slots. */

void UiGauntletSetupRefreshSlotSprites(UiGauntletSetup *self)
{
  GfxSprite *sprite;
  int i;

  for (i = 10; i < 0xe; i++) {
    UiGauntletSetupResetSprite(self, (u16)i);
    if ((self->slotMark[i - 10] & 1) == 0) {
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags &= ~1u;
    }
  }
  for (i = 0x32; i < 0x36; i++) {
    UiGauntletSetupResetSprite(self, (u16)i);
    if ((self->slotMark[i - 0x32] & 1) == 0) {
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags &= ~1u;
    }
  }
  for (i = 0x28; i < 0x2a; i++) {
    UiGauntletSetupResetSprite(self, (u16)i);
    sprite = ((GfxSprite **)self->base.data)[i];
    if (self->chosenCard[i - 0x28] == 0xff) {
      sprite->flags &= ~1u;
    }
    else {
      UiCardSetThumbnailTexture(sprite, self->chosenCard[i - 0x28]);
    }
  }
  for (i = 0x18; i < 0x1c; i++) {
    UiGauntletSetupResetSprite(self, (u16)i);
    UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 2);
    if ((self->slotMark[i - 0x18] & 1) == 0) {
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags &= ~1u;
    }
  }
}
