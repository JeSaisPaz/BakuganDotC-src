// bdc 0x0891f984 UiHologramGalleryInfoPanelDone
#include "bdc.h"

/* Advances the tweens (mode 9) of the info-panel sprites listed by
   `UiHologramGalleryInfoPanelSprite` and returns true once any of them has
   finished (`UiTweenUpdate` returns true on completion). */

bool UiHologramGalleryInfoPanelDone(UiHologramGallery *self, u8 hide)
{
  u8 finished = 0;
  u8 n = 0;
  u32 idx;

  while (1) {
    idx = UiHologramGalleryInfoPanelSprite(self, n) & 0xffff;
    if (idx == 0xff) {
      break;
    }
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide,
                              ((GfxSprite **)self->base.data)[idx],
                              &self->tweens[idx], 9);
    n++;
  }
  return finished != 0;
}
