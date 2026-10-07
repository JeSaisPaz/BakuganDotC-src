// bdc 0x0891b9c0 UiHologramGalleryPickMenuMode
#include "bdc.h"

/* Chooses the menu mode `+0x2176` of the hologram gallery screen (`UiHologramGalleryCtor`, task
   391; menu cursor `+0x77`, panel `+0x74`): 1 normally; while the tutorial state holds (profile
   word 0x2b == 2 and word 0x2e == 0) 0 for battle ids 0..1, 2 from id 2 on, and 0 again in battle 4
   until profile flag `+0x82` bit 0 is set. */

void UiHologramGalleryPickMenuMode(UiHologramGallery *self)
{
  s8 mode;

  self->menuMode = 1;
  if (SaveProfileGetWord(SaveGetProfile(), 0x2b) == 2) {
    if (SaveProfileGetWord(SaveGetProfile(), 0x2e) == 0) {
      mode = 0;
      if (g_scriptGlobalVars[1] >= 2 && (mode = 2, g_scriptGlobalVars[1] == 4)) {
        mode = 2;
        if ((SaveGetProfile()->data->viewSeenMask & 1) == 0) {
          mode = 0;
        }
      }
      self->menuMode = mode;
    }
  }
}
