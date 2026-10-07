// bdc 0x0892d66c UiBakuganSelectTweenHelpPanel
#include "bdc.h"

/* Starts the appear tweens of the help-panel sprites 0x5a..0x5d and sprite 1 of the Bakugan
   select screen (`UiBakuganSelectCtor`): each is made visible (`flags` bit 0) on layer mask 4,
   sprite 0x5a shows button icon 2 and 0x5b button icon 1 (`UiSetButtonIcon`). When `hide` is
   set only the disappear tweens are started on the same sprites. */

void UiBakuganSelectTweenHelpPanel(UiBakuganSelect *self, u8 hide)
{
  int i;

  if (hide == 0) {
    for (i = 0x5a; i < 0x5e; i++) {
      if (i == 0x5a) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 2);
      } else if (i == 0x5b) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 1);
      }
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 4;
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 1; i < 2; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 4;
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  } else {
    for (i = 0x5a; i < 0x5e; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 1; i < 2; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}
