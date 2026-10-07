// bdc 0x0892050c UiHologramGalleryStartCursorPulse
#include "bdc.h"

/* Starts the highlight pulse (`UiPulseStepTint`, period 40) on the cursor of the active panel `+0x74`
   of the hologram gallery screen (`UiHologramGalleryCtor`, task 391; menu cursor `+0x77`, panel
   `+0x74`) (e.g. panel 0: sprite `+0x20c` for menu items ≥ 2, else `+0x2d8`). */

void UiHologramGalleryStartCursorPulse(UiHologramGallery *self)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  s8 panel = self->panel;

  if (panel <= 0) {
    if (panel >= 0) {
      if (self->menuCursor >= 2) {
        UiPulseStepTint(40.0f, sprites[0x83], (UiPulse *)&self->tweens[0x83]);
        return;
      }
      UiPulseStepTint(40.0f, sprites[0xb6], (UiPulse *)&self->tweens[0xb6]);
      return;
    }
  } else {
    if (panel < 2) {
      if (self->listMode[0] != 0) {
        UiPulseStepTint(40.0f, sprites[0x83], (UiPulse *)&self->tweens[0x83]);
        return;
      }
      UiPulseStepTint(40.0f, sprites[0x6b], (UiPulse *)&self->tweens[0x6b]);
      return;
    }
    if (panel < 3) {
      UiPulseStepTint(40.0f, sprites[0x8c], (UiPulse *)&self->tweens[0x8c]);
    }
  }
}
