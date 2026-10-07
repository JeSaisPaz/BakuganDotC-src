// bdc 0x08913874 UiUpgradeSlotPanelDone
#include "bdc.h"

/* Advances the slot-panel tweens of the Bakugan upgrade screen (task 490,
   `maybe_UiScreen490Ctor`; `"up_grade.fab"`, `"DMUpgrade"` texts) by one frame
   (`UiTweenUpdate`, scale 1 -> 1 over 16 frames, `hide` as fade-out): sprites 0xd, 0xf and 0x27
   with flags 1, and the three 7-sprite digit groups 0x11..0x17, 0x18..0x1e, 0x1f..0x25 with
   flags 9. Returns true once any tween reports finished (u8 sum of the results != 0; all share
   the same length, so in practice once all have finished). */

bool UiUpgradeSlotPanelDone(UiUpgrade *self, u8 hide)
{
  u8 busy;
  int i;

  busy = 0;
  for (i = 0xd; i < 0xe; i++) {
    busy += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  }
  for (i = 0xf; i < 0x10; i++) {
    busy += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  }
  for (i = 0x27; i < 0x28; i++) {
    busy += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  }
  for (i = 0x11; i < 0x18; i++) {
    busy += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 9);
  }
  for (i = 0x18; i < 0x1f; i++) {
    busy += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 9);
  }
  for (i = 0x1f; i < 0x26; i++) {
    busy += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 9);
  }
  return busy != 0;
}
