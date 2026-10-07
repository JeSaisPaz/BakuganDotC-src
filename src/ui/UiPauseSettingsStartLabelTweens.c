// bdc 0x089ac74c UiPauseSettingsStartLabelTweens
#include "bdc.h"

/* Shows the four row labels (layout entries 0x1b..0x1e) and starts their tweens (`labelTweens`, +0x4b0). */

void UiPauseSettingsStartLabelTweens(UiPauseSettings *self, u8 closing)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  int i;

  for (i = 0x1b; i < 0x1f; i++) {
    sprites[i]->flags |= 1;
    UiTweenBegin(1.5f, closing, sprites[i], &self->labelTweens[i - 0x1b], 1);
  }
}
