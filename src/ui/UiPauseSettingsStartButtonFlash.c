// bdc 0x089ad4a0 UiPauseSettingsStartButtonFlash
#include "bdc.h"

/* Starts the 4-frame decide flash (`UiFlashStart`, slot 0) on the selected button
   (`data+0xb8+item*4`). */

void UiPauseSettingsStartButtonFlash(UiPauseSettings *self)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  UiFlashStart(4.0f, sprites[0xb8 / 4 + self->cursor], 0, 0);
}
