// bdc 0x08921644 UiHologramGalleryRefreshMenu
#include "bdc.h"

/* Rebuilds the command menu of the hologram gallery screen (`UiHologramGalleryCtor`, task 391;
   menu cursor `+0x77`, panel `+0x74`) for the active panel: panel 0 re-picks the mode
   (`UiHologramGalleryPickMenuMode`) and lays it out (`UiHologramGallerySetupMenu`); panels 1..2
   use mode 0 while tutorial-locked, else 1. */

void UiHologramGalleryRefreshMenu(UiHologramGallery *self)

{
  s8 panel = self->panel;

  if (panel <= 0) {
    if (panel >= 0) {
      UiHologramGalleryPickMenuMode(self);
      UiHologramGallerySetupMenu(self, self->menuMode);
    }
  } else if (panel < 3) {
    if (UiHologramGalleryIsTutorialLocked() == 1) {
      UiHologramGallerySetupMenu(self, 0);
    } else {
      UiHologramGallerySetupMenu(self, 1);
    }
  }
  return;
}
