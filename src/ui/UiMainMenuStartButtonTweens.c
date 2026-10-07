// bdc 0x089a9014 UiMainMenuStartButtonTweens
#include "bdc.h"

/* Starts the `UiTween`s (`slots[15..18]`, `UiTweenBegin` scale 1.0, flags 3) of layout sprites
   15..18 for the opening (`closing` 0) or closing animation. When opening, first sets the button
   icons of sprites 15 (icon 2) and 16 (icon 1) (`UiSetButtonIcon`) and shows all four sprites. */

void UiMainMenuStartButtonTweens(UiMainMenu *self, u8 closing)
{
  int i;

  if (closing == 0) {
    for (i = 15; i < 19; i++) {
      if (i == 15) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 2);
      }
      else if (i == 16) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 1);
      }
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBegin(1.0f, closing, ((GfxSprite **)self->base.data)[i], &self->slots[i].tween, 3);
    }
  }
  else {
    for (i = 15; i < 19; i++) {
      UiTweenBegin(1.0f, closing, ((GfxSprite **)self->base.data)[i], &self->slots[i].tween, 3);
    }
  }
  return;
}
