// bdc 0x0891a790 UiAdvSelectFlashSelection
#include "bdc.h"

/* Starts the decide flash (`UiFlashStart`, 4.0) on the selected candidate's two sprites (slots
   5+cursor and 11+cursor of the screen data pointer array). */

void UiAdvSelectFlashSelection(UiAdvSelect *self)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  UiFlashStart(4.0f, sprites[5 + self->cursor], 0, 0);
  UiFlashStart(4.0f, sprites[11 + self->cursor], 0, 1);
}
