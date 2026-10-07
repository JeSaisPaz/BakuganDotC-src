// bdc 0x08913d80 UiUpgradeHeaderPanelsDone
#include "bdc.h"

/* Advances the tweens[5..9] transitions of the header panel sprites data[5..9] of the Bakugan
   upgrade screen (`UiUpgradeCtor`, task 490) by one frame through `UiTweenUpdate` (16 frames,
   slide mode 1, `hide` selects fade-out) and returns true when at least one of them has finished
   (`UiTweenUpdate` returns true once done), false while all five are still running.
   Counterpart of `UiUpgradeTweenHeaderPanels`. */

bool UiUpgradeHeaderPanelsDone(UiUpgrade *self, u8 hide)
{
  GfxSprite **sprites;
  u8 finished;
  int i;

  finished = 0;
  for (i = 5; i < 10; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished = finished + UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
  }
  return finished != 0;
}
