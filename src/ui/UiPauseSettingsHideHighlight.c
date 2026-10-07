// bdc 0x089ad744 UiPauseSettingsHideHighlight
#include "bdc.h"

/* Hides the button highlight (`data+0xe0`) and the pulse ghost (`data+0xfc`). */

void UiPauseSettingsHideHighlight(UiPauseSettings *self)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  sprites[0xe0 / 4]->flags &= ~1u;
  sprites[0xfc / 4]->flags &= ~1u;
}
