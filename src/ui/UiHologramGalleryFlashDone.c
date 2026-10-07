// bdc 0x08920b50 UiHologramGalleryFlashDone
#include "bdc.h"

/* Advances the decide flash channels (`UiFlashStep`) for the active panel of the hologram gallery
   screen (`UiHologramGalleryCtor`, task 391; menu cursor `+0x77`, panel `+0x74`) and returns 1
   once channel 0 has completed, 0 while it is running (or for an invalid panel). */

int UiHologramGalleryFlashDone(UiHologramGallery *self)
{
    s8 panel = self->panel;

    if (panel > 0) {
        if (panel < 2) {
            if (UiFlashStep(0) != 0) {
                return 1;
            }
        } else if (panel < 3) {
            UiFlashStep(1);
            if (UiFlashStep(0) != 0) {
                return 1;
            }
        }
    } else if (panel >= 0) {
        if (self->menuCursor < 2) {
            UiFlashStep(1);
        }
        if (UiFlashStep(0) != 0) {
            return 1;
        }
    }
    return 0;
}
