// bdc 0x0891f020 UiHologramGalleryTweenBase
#include "bdc.h"

/* Starts the open tweens (1.5 s, mode 3) of the base sprites 0..4 of the hologram gallery screen
   (`UiHologramGalleryCtor`, task 391; menu cursor `+0x77`, panel `+0x74`) (records `+0x7c..`), or
   their reverse when `hide` is set. */

void UiHologramGalleryTweenBase(UiHologramGallery *self, u8 hide)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  int i;

  if (hide == 0) {
    for (i = 0; i < 5; i++) {
      sprites[i]->flags |= 1;
      UiTweenBegin(1.5f, 0, sprites[i], &self->tweens[i], 3);
    }
  } else {
    for (i = 0; i < 5; i++) {
      UiTweenBegin(1.5f, hide, sprites[i], &self->tweens[i], 3);
    }
  }
}
