// bdc 0x0891352c UiUpgradeBasePanelsDone
#include "bdc.h"

/* Advances by one frame the base-panel tweens started by `UiUpgradeTweenBasePanels` on the
   Bakugan upgrade screen: sprites/tweens 0..4 with the pulse helper `UiSpriteEaseStep` (alpha
   step 0.3, flags 3), sprite 10 with `UiTweenUpdate` flags 7 and 11..12 with flags 0xb, scaling
   1.5 -> 1.0 when showing (`hide == 0`) or 1.0 -> 1.5 when hiding, over 16 frames. Returns true
   when the 8-bit sum of the per-tween results (each true once that tween finished) is non-zero. */

bool UiUpgradeBasePanelsDone(UiUpgrade *self, u8 hide)
{
  u8 finished;
  int i;

  finished = 0;
  if (hide == 0) {
    for (i = 0; i < 5; i++) {
      finished += UiSpriteEaseStep(1.5f, 1.0f, 16.0f, 0.3f, hide,
                                   ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 10; i < 11; i++) {
      finished += UiTweenUpdate(1.5f, 1.0f, 16.0f, hide,
                                ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
    }
    for (i = 11; i < 12; i++) {
      finished += UiTweenUpdate(1.5f, 1.0f, 16.0f, hide,
                                ((GfxSprite **)self->base.data)[i], &self->tweens[i], 0xb);
    }
    for (i = 12; i < 13; i++) {
      finished += UiTweenUpdate(1.5f, 1.0f, 16.0f, hide,
                                ((GfxSprite **)self->base.data)[i], &self->tweens[i], 0xb);
    }
  }
  else {
    for (i = 0; i < 5; i++) {
      finished += UiSpriteEaseStep(1.0f, 1.5f, 16.0f, 0.3f, hide,
                                   ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 10; i < 11; i++) {
      finished += UiTweenUpdate(1.0f, 1.5f, 16.0f, hide,
                                ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
    }
    for (i = 11; i < 12; i++) {
      finished += UiTweenUpdate(1.0f, 1.5f, 16.0f, hide,
                                ((GfxSprite **)self->base.data)[i], &self->tweens[i], 0xb);
    }
    for (i = 12; i < 13; i++) {
      finished += UiTweenUpdate(1.0f, 1.5f, 16.0f, hide,
                                ((GfxSprite **)self->base.data)[i], &self->tweens[i], 0xb);
    }
  }
  return finished != 0;
}
