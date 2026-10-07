// bdc 0x08929ea4 UiHologramViewInitTipSprites
#include "bdc.h"

/* Prepares the tip sprites 0x14.. of the hologram detail view (`UiHologramViewCtor`, task 392;
   view kind `kind`): sprite 0x15 gets face 1 when the first page id `pageIds[0]` is 0x12, else face
   0 (`UiHologramViewSetFacePicture`); all start transparent. */

void UiHologramViewInitTipSprites(UiHologramView *self)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  int i;

  for (i = 0x14; i < 0x18; i++) {
    if (i == 0x15) {
      UiHologramViewSetFacePicture(self, sprites[i], self->pageIds[0] == 0x12 ? 1 : 0);
    }
    sprites[i]->alpha = 0.0f;
    sprites[i]->flags |= 1;
  }
}
