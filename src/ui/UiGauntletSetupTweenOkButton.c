// bdc 0x0893432c UiGauntletSetupTweenOkButton
#include "bdc.h"

/* Starts the open (`closing` = 0) or close pop tweens (`UiTweenBegin`, 1.0, mode 3) of the OK
   button of `UiGauntletSetup`: sprite 0x26 (tween 0x26, set to the unlit
   texture with `UiGauntletSetupSetOkButtonTexture`) and sprite 0x2f (tween 0x2f), made
   visible with layer mask 2 on open. */

void UiGauntletSetupTweenOkButton(UiGauntletSetup *self, u8 closing)
{
  if (closing == 0) {
    ((GfxSprite **)self->base.data)[0x26]->flags |= 1;
    ((GfxSprite **)self->base.data)[0x26]->layerMask = 2;
    UiGauntletSetupSetOkButtonTexture(self, ((GfxSprite **)self->base.data)[0x26], 0);
    UiTweenBegin(1.0f, closing, ((GfxSprite **)self->base.data)[0x26], &self->tweens[0x26], 3);
    ((GfxSprite **)self->base.data)[0x2f]->flags |= 1;
    ((GfxSprite **)self->base.data)[0x2f]->layerMask = 2;
    UiTweenBegin(1.0f, closing, ((GfxSprite **)self->base.data)[0x2f], &self->tweens[0x2f], 3);
  } else {
    UiTweenBegin(1.0f, closing, ((GfxSprite **)self->base.data)[0x26], &self->tweens[0x26], 3);
    UiTweenBegin(1.0f, closing, ((GfxSprite **)self->base.data)[0x2f], &self->tweens[0x2f], 3);
  }
}
