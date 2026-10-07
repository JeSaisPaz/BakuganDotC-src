// bdc 0x0892043c UiHologramGalleryHideCursor
#include "bdc.h"

/* Hides the cursor sprites of the active panel `panel` of the hologram gallery screen
   (`UiHologramGalleryCtor`, task 391; menu cursor `menuCursor`): panel 0 clears the visible flag
   (bit 0 of `flags`) of sprites 182, 131 and 188, panel 1 of 107, 131 and 188, panel 2 of 140 and
   188, any other panel (negative or above 2) of 188 only. */

void UiHologramGalleryHideCursor(UiHologramGallery *self)
{
  UiHologramGalleryData *data;
  s8 panel;

  panel = self->panel;
  data = (UiHologramGalleryData *)self->base.data;
  if (panel > 0) {
    if (panel < 2) {
      data->sprites[107]->flags &= ~1u;
      ((UiHologramGalleryData *)self->base.data)->sprites[131]->flags &= ~1u;
    } else if (panel < 3) {
      data->sprites[140]->flags &= ~1u;
    }
  } else if (panel >= 0) {
    data->sprites[182]->flags &= ~1u;
    ((UiHologramGalleryData *)self->base.data)->sprites[131]->flags &= ~1u;
  }
  ((UiHologramGalleryData *)self->base.data)->sprites[188]->flags &= ~1u;
}
