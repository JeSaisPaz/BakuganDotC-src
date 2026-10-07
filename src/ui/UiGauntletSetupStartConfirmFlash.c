// bdc 0x08935660 UiGauntletSetupStartConfirmFlash
#include "bdc.h"

/* Starts the confirm flash of `UiGauntletSetup` on the focused item
   (`UiFlashStart`, 4 frames): on the slots, slot sprite 0x0e+slot (flash channel 0) and sprite
   0x2a+slot (channel 1); on the OK button, sprite 0x26 (channel 0). */

void UiGauntletSetupStartConfirmFlash(UiGauntletSetup *self)

{
  GfxSprite **spr = (GfxSprite **)self->base.data;

  if (self->focusArea == 0) {
    UiFlashStart(4.0f, spr[0x0e + self->item], 0, 0);
    UiFlashStart(4.0f, ((GfxSprite **)self->base.data)[0x2a + self->item], 0, 1);
  }
  else {
    UiFlashStart(4.0f, spr[0x26], 0, 0);
  }
}
