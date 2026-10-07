// bdc 0x089a7d64 UiMainMenuSetNetSpritesVisible
#include "bdc.h"

/* Shows (`show` 1) or hides the four sprites `data+0x3c..+0x48` (layout entries 15..18, the
   net/ad-hoc indicators). */

void UiMainMenuSetNetSpritesVisible(UiMainMenu *self, u8 show)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  int i;

  if (show == 1) {
    for (i = 15; i < 19; i++) {
      sprites[i]->flags |= 1;
    }
  }
  else {
    for (i = 15; i < 19; i++) {
      sprites[i]->flags &= ~1u;
    }
  }
  return;
}
