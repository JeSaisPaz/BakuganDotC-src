// bdc 0x08919628 UiAdvSelectTweenHelpPanel
#include "bdc.h"

/* Starts the mode-3 tweens (`tweens[0x22..0x24]`) of sprites 0x22..0x24 of the adventure
   partner-select screen (`UiAdvSelectCtor`, task 376). When `hide` is 0 each sprite is first made
   visible (flags bit 0) and sprite 0x23 gets button icon 2 via `UiSetButtonIcon`, then fades in;
   when `hide` is set the three sprites fade out. */

void UiAdvSelectTweenHelpPanel(UiAdvSelect *self, u8 hide)
{
  GfxSprite *sprite;
  int i;

  if (hide == 0) {
    for (i = 0x22; i < 0x25; i++) {
      sprite = ((GfxSprite **)self->base.data)[i];
      if (i == 0x23) {
        UiSetButtonIcon(sprite, 2);
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      sprite->flags |= 1;
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  } else {
    for (i = 0x22; i < 0x25; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}
