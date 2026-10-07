// bdc 0x08920608 UiHologramGalleryStopCursorPulse
#include "bdc.h"

/* Stops the highlight pulse (`UiCursorGlowStep`) on the selected item of the active panel `+0x74` of
   the hologram gallery screen (`UiHologramGalleryCtor`, task 391; menu cursor `+0x77`, panel
   `+0x74`). */

void UiHologramGalleryStopCursorPulse(UiHologramGallery *self)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  s32 panel = self->panel;

  if (panel == 0) {
    if (self->menuCursor < 2) {
      UiCursorGlowStep(sprites[(&self->menuCursor)[panel] + 0xb4]);
      return;
    }
    UiCursorGlowStep(sprites[0x80]);
  } else if (panel == 1) {
    if (self->listMode[0] == 0) {
      UiCursorGlowStep(sprites[(&self->menuCursor)[panel] + 0x67]);
      return;
    }
    UiCursorGlowStep(sprites[0x80]);
  }
  return;
}
