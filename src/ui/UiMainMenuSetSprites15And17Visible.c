// bdc 0x089a809c UiMainMenuSetSprites15And17Visible
#include "bdc.h"

/* Shows (`show` 1) or hides layout sprites 15 and 17 (`data+0x3c`, `data+0x44`). */

void UiMainMenuSetSprites15And17Visible(UiMainMenu *self, u8 show)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  if (show == 1) {
    sprites[15]->flags |= 1;
    sprites[17]->flags |= 1;
    return;
  }
  sprites[15]->flags &= ~1u;
  sprites[17]->flags &= ~1u;
  return;
}
