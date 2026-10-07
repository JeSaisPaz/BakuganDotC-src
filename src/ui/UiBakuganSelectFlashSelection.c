// bdc 0x0892f3c8 UiBakuganSelectFlashSelection
#include "bdc.h"

/* Starts the decide flash (`UiFlashStart`, 4.0) on the two sprites of the selected grid cell
   (`+0x68`, `+0x108` + 4*cursor). */

void UiBakuganSelectFlashSelection(UiBakuganSelect *self)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  UiFlashStart(4.0f, sprites[26 + self->cursor], 0, 0);
  UiFlashStart(4.0f, sprites[66 + self->cursor], 0, 1);
}
