// bdc 0x08933804 UiGauntletSetupTweenFrame
#include "bdc.h"

/* Starts the open (`hide` = 0) or close pop tweens (`UiTweenBegin`, 1.0, mode 3) of the frame
   sprites of the gauntlet setup screen (`UiGauntletSetup`): sprites
   0x1c..0x21 and sprite 0 (each with its own tween slot). On open the sprites are made visible
   with layer mask 4, and sprites 0x1c/0x1d first get button icons 2/1 (`UiSetButtonIcon`). */

void UiGauntletSetupTweenFrame(UiGauntletSetup *self, u8 hide)
{
  int i;

  if (hide == 0) {
    for (i = 0x1c; i < 0x22; i++) {
      if (i == 0x1c) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 2);
      } else if (i == 0x1d) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 1);
      }
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 4;
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    ((GfxSprite **)self->base.data)[0]->flags |= 1;
    ((GfxSprite **)self->base.data)[0]->layerMask = 4;
    UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[0], &self->tweens[0], 3);
  } else {
    for (i = 0x1c; i < 0x22; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[0], &self->tweens[0], 3);
  }
}
