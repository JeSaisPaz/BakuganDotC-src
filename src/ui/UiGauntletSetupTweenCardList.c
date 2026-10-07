// bdc 0x08933d64 UiGauntletSetupTweenCardList
#include "bdc.h"

/* Starts the open/close tweens of the card list of the gauntlet setup screen (task 373,
   `maybe_UiScreen373Ctor`, class prefix `UiGauntletSetup`; card sprites `cc_card_L_%03d`,
   `c_set_OK_bo_*`, texts `DWCardName`/`DWCardHelp`; main update `UiGauntletSetupMainPhase`; focus area `+0x74`,
   item `+0x76`). Sprites 7..9, 48..49 and 40..41 each get tween `tweens[i]` (`UiTweenBegin`, flags 3).
   Opening (`hide == 0`): the sprites are made visible (flags bit 0) on layer 2 and start from scale 0;
   sprites 40/41 show the chosen cards (`chosenCard[0..1]`, `UiCardSetThumbnailTexture`) or are hidden
   when the slot is empty (0xff); then the list is laid out (`UiGauntletSetupLayoutCardList`).
   Closing: the same sprites fade out from scale 1. The sprite table is re-read from `base.data` on
   every access. */

void UiGauntletSetupTweenCardList(UiGauntletSetup *self, u8 hide)
{
  GfxSprite *sprite;
  int i;

  if (hide == 0) {
    for (i = 7; i < 10; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 2;
      UiTweenBegin(0.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 48; i < 50; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 2;
      UiTweenBegin(0.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 40; i < 42; i++) {
      sprite = ((GfxSprite **)self->base.data)[i];
      if (self->chosenCard[i - 40] != 0xff) {
        sprite->flags |= 1;
        UiCardSetThumbnailTexture(((GfxSprite **)self->base.data)[i], self->chosenCard[i - 40]);
        sprite = ((GfxSprite **)self->base.data)[i];
      } else {
        sprite->flags &= ~1u;
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      sprite->layerMask = 2;
      UiTweenBegin(0.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    UiGauntletSetupLayoutCardList(self);
  } else {
    for (i = 7; i < 10; i++)
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    for (i = 48; i < 50; i++)
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    for (i = 40; i < 42; i++)
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
  }
}
